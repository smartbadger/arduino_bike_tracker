#pragma once

#ifndef _GPS_H
#define _GPS_H

#include <NMEAGPS.h>
#include "debugger.h"
#include <GPSport.h>
#include "StateMachine/StateMachine.h"
namespace GPS
{
    static NMEAGPS neoGPS; // This parses the GPS characters

    static void displaySatellitesInView()
    {
        debugV(neoGPS.sat_count);
        debugV(',');

        for (uint8_t i = 0; i < neoGPS.sat_count; i++)
        {
            debugV(neoGPS.satellites[i].id);
            debugV(' ');
            debugV(neoGPS.satellites[i].elevation);
            debugV('/');
            debugV(neoGPS.satellites[i].azimuth);
            debugV('@');
            debugV(", ");
        }

        debuglnV("");
    }
    static gps_fix doSomeWork(const gps_fix &fix, State &state)
    {
        //  This is the best place to do your time-consuming work, right after
        //     the RMC sentence was received.  If you do anything in "loop()",
        //     you could cause GPS characters to be lost, and you will not
        //     get a good lat/lon.
        //  For this example, we just print the lat/lon.  If you print too much,
        //     this routine will not get back to "loop()" in time to process
        //     the next set of GPS data.

        if (fix.valid.location)
        {
            debuglnV("GPS found");
            uint8_t sat = -1;
            if (fix.valid.satellites)
            {
                sat = fix.satellites;
            }
            state.setGPS({fix.latitude(), fix.longitude(), fix.altitude_cm(), sat}, sat, fix.speed_mph(), fix.dateTime);
        }
        else
        {
            debuglnV("GPS not valid");
        }

    } // doSomeWork

    //------------------------------------

    static void GPSloop(State &data)
    {
        while (neoGPS.available(gpsPort))
            doSomeWork(neoGPS.read(), data);

    } // GPSloop

    //--------------------------

    void setup()
    {
        debuglnV("Looking for GPS device on " GPS_PORT_NAME);
        gpsPort.begin(9600);
    }

    //--------------------------
}
#endif