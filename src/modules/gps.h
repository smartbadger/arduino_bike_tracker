#pragma once

#ifndef _GPS_H
#define _GPS_H

#include <NMEAGPS.h>
#include "observer.h"
#include "debugger.h"
#include <GPSport.h>
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

    } // displaySatellitesInView
    static gps_fix doSomeWork(const gps_fix &fix, BikeDataObservable &data)
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
            uint8_t sat = -1;
            if (fix.valid.satellites)
                sat = fix.satellites;
            data.setGPS({fix.latitude(), fix.longitude(), fix.altitude_cm(), sat}, sat, fix.speed_mph(), fix.dateTime);
        }
        else
        {
            debuglnV("GPS not valid");
        }

    } // doSomeWork

    //------------------------------------

    static void GPSloop(BikeDataObservable &data)
    {
        while (neoGPS.available(gpsPort))
            doSomeWork(neoGPS.read(), data);

    } // GPSloop

    //--------------------------

    void setup()
    {
        debugV("NMEAloc.INO: started\n");
        // debugV("fix object size = ");
        // debuglnV(sizeof(neoGPS.fix());
        debugV("NMEAGPS object size = ");
        // debuglnV(sizeof(neoGPS);
        debuglnV("Looking for GPS device on " GPS_PORT_NAME);

        gpsPort.begin(9600);
    }

    //--------------------------
}
#endif