/* -*- c++ -*-
 *
 * DCCEXProtocol
 *
 * This package implements a DCC-EX protocol connection,
 * allow a device to communicate with a DCC-EX EX CommnadStation
 *
 * Copyright © 2023 - Peter Akers
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to
 * Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
 *
 * Attribution — You must give appropriate credit, provide a link to the
 * license, and indicate if changes were made. You may do so in any
 * reasonable manner, but not in any way that suggests the licensor
 * endorses you or your use.
 *
 * ShareAlike — If you remix, transform, or build upon the material, you
 * must distribute your contributions under the same license as the
 * original.
 *
 * All other rights reserved.
 *
 */

//#include <ArduinoTime.h>
//#include <TimeLib.h>
#include <vector>  //https://github.com/arduino-libraries/Arduino_AVRSTL

#include "DCCEXProtocol.h"

static const int MIN_SPEED = 0;
static const int MAX_SPEED = 126;
static const char *rosterSegmentDesc[] = {"Name", "Address", "Length"};


DCCEXProtocol::DCCEXProtocol(bool server) {
	// store server/client
    this->server = server;
		
	// init streams
    stream = &nullStream;
	console = &nullStream;
}

// init the DCCEXProtocol instance after connection to the server
void DCCEXProtocol::init() {
    console->println("init()");
    
	// allocate input buffer and init position variable
	memset(inputbuffer, 0, sizeof(inputbuffer));
	nextChar = 0;
	
    //last Response time
    lastServerResponseTime = millis() /1000;
	
	// init change flags
    resetChangeFlags();

    console->println("init(): end");
}


// Set the delegate instance for callbasks
void DCCEXProtocol::setDelegate(DCCEXProtocolDelegate *delegate) {
    this->delegate = delegate;
}


// Set the Stream used for logging
void DCCEXProtocol::setLogStream(Stream *console) {
    this->console = console;
}


void DCCEXProtocol::connect(Stream *stream) {
    init();
    this->stream = stream;
}

void DCCEXProtocol::disconnect() {
    String command = "Q";
    sendCommand(command);
    this->stream = NULL;
}

bool DCCEXProtocol::check() {
    bool changed = false;

    if (stream) {
        while(stream->available()) {
            char b = stream->read();
            if (b == NEWLINE || b==CR) {
                // server sends TWO newlines after each command, we trigger on the
                // first, and this skips the second one
                if (nextChar != 0) {
                    inputbuffer[nextChar] = 0;
                    changed |= processCommand(inputbuffer, nextChar);
                }
                nextChar = 0;
            }
            else {
                inputbuffer[nextChar] = b;
                nextChar += 1;
                if (nextChar == (sizeof(inputbuffer)-1) ) {
                    inputbuffer[sizeof(inputbuffer)-1] = 0;
                    console->print("ERROR LINE TOO LONG: >");
                    console->print(sizeof(inputbuffer));
                    console->print(": ");
                    console->println(inputbuffer);
                    nextChar = 0;
                }
            }
        }

        return changed;

    }
    else {
        return false;
    }
}

void DCCEXProtocol::sendCommand(String cmd) {
    if (stream) {
        // TODO: what happens when the write fails?
        stream->println(cmd);
        if (server) {
            stream->println("");
        }
        console->print("==> "); console->println(cmd);
    }
}



bool DCCEXProtocol::processCommand(char *c, int len) {
    bool changed = false;

    console->print("<== ");
    console->println(c);

    lastServerResponseTime = millis()/1000;

    return false;
}


bool DCCEXProtocol::processLocoAction(char *c, int len) {
}


void DCCEXProtocol::processServerDescription(char *c, int len) {
    if (delegate && len > 0) {
        String serverDescription = String(c);
        delegate->receivedServerDescription(serverDescription);
    }
}


void DCCEXProtocol::processRosterList(char *c, int len) {
    console->println("processRosterList()");
    console->println("processRosterList(): end");
}


void DCCEXProtocol::processTurnoutList(char *c, int len) {
    console->println("processTurnoutList()");
    console->println("processTurnoutList(): end");
}


void DCCEXProtocol::processRouteList(char *c, int len) {
    console->println("processRouteList()");
    console->println("processRouteList(): end");
}

void DCCEXProtocol::processTrackPower(char *c, int len) {
    console->println("processTrackPower()");

    if (delegate) {
        if (len > 0) {
            TrackPower state = PowerUnknown;
            if (c[0]=='0') {
                state = PowerOff;
            }
            else if (c[0]=='1') {
                state = PowerOn;
            }

            delegate->receivedTrackPower(state);
        }
    }
}


void DCCEXProtocol::processAddRemove(char multiThrottle, char *c, int len) {
    console->print("processAddRemove(): "); console->println(multiThrottle);

    if (!delegate) {
        // If no one is listening, don't do the work to parse the string
        return;
    }

    //console->printf("processing add/remove command %s\n", c);

    String s(c);

    bool add = (c[0] == '+');
    bool remove = (c[0] == '-');

    int p = s.indexOf(PROPERTY_SEPARATOR);
    if (p > 0) {
        String address = s.substring(1, p);
        String entry   = s.substring(p+3);

        address.trim();
        entry.trim();

        if (add) {
            if (multiThrottle == DEFAULT_MULTITHROTTLE) {
                delegate->addressAdded(address, entry);
            } else {
                delegate->addressAddedMultiThrottle(multiThrottle, address, entry);
            }
        }
        if (remove) {
            if (entry.equals("d\n") || entry.equals("r\n")) {
                delegate->addressRemoved(address, entry);
            }
            else {
                console->printf("malformed address removal: command is %s\n", entry.c_str());
                console->printf("entry length is %d\n", entry.length());
                for (int i = 0; i < entry.length(); i++) {
                    console->printf("  char at %d is %d\n", i, entry.charAt(i));
                }
            }
        }
    }

    console->println("processAddRemove(): end"); 
}


void DCCEXProtocol::processTurnoutAction(char *c, int len) {
    console->println("processTurnoutAction(): ");
    if (delegate) {

    }
    console->println("processTurnoutAction(): end");
}


void DCCEXProtocol::processRouteAction(char *c, int len) {
    console->println("processRouteAction(): ");
    if (delegate) {

    }
    console->println("processRouteAction(): end");
}


void DCCEXProtocol::processTurntableAction(char *c, int len) {
    console->println("processTurntableAction(): ");
    if (delegate) {

    }
    console->println("processTurntableAction(): end");
}


// ******************************************************************************************************

bool DCCEXProtocol::addLocomotive(int throttle, int address) {
    console->print("addLocomotive(): "); console->print(throttle); console->print(" : "); console->println(address);
    bool ok = false;
    return ok;
}

// ******************************************************************************************************

bool DCCEXProtocol::releaseLocomotive(int throttle, String address) {
    console->print("releaseLocomotive(): "); console->print(throttle); console->print(" : "); console->println(address);
    return true;
}

// ******************************************************************************************************

int DCCEXProtocol::getLocomotiveAtPosition(int throttle, int position) {
    console->print("getLocomotiveAtPosition(): "); console->print(throttle); console->print(" : "); console->println(position);
    return {};
}

// ******************************************************************************************************

int DCCEXProtocol::getNumberOfLocos(int throttle) {
    console->print("getNumberOfLocos(): "); console->println(throttle);
    int size = 0;
    console->print("getNumberOfLocomotives(): end "); console->println(size);
    return size;
}

// ******************************************************************************************************

bool DCCEXProtocol::setSpeed(int throttle, int speed) {
    console->print("setSpeed(): "); console->print(throttle); console->print(" : "); console->println(speed);
    console->println("setSpeed(): end"); 
    return true;
}

// ******************************************************************************************************

void requestLocoUpdate(int address) {

}

int DCCEXProtocol::setLoco(int address, int speed, Direction direction) {
    console->print("setLoco(): "); console->println(address);
    console->println("setLoco(): end");
}


// ******************************************************************************************************

void DCCEXProtocol::emergencyStop() {
    emergencyStop(DEFAULT_MULTITHROTTLE, ALL_LOCOS_ON_THROTTLE);
}


// ******************************************************************************************************

void DCCEXProtocol::setFunction(int throttle, String address, int funcNum, bool pressed) {
    console->print("setFunction(): "); console->print(multiThrottle); console->print(" : "); console->println(funcNum);

    sendCommand(cmd);

    console->println("setFunction(): end"); 
}

// ******************************************************************************************************

void DCCEXProtocol::setTrackPower(TrackPower state) {

    String cmd = "PPA";
    cmd.concat(state);

    sendCommand(cmd);	
}

bool DCCEXProtocol::setTurnout(int turnoutId, TurnoutAction action) {  // address is turnout system name
    sendCommand(cmd);

    return true;
}

bool DCCEXProtocol::setRoute(int routeId) {  // address is turnout system name
    String cmd = "";
    sendCommand(cmd);
    return true;
}

long DCCEXProtocol::getLastServerResponseTime() {
  return lastServerResponseTime;   
}


bool setTurntable(int TurntableId, int position, int activity) {

}

bool setAccessory(int accessoryAddress, int activate) {

}

bool setAccessory(int accessoryAddress, int accessorySubAddr, int activate) {
    
}
