#include <NMEAGPS.h>
#include "models.h"

//======================================================================
//  Program: NMEAloc.ino
//
//  Description:  This program only parses an RMC sentence for the lat/lon.
//
//  Prerequisites:
//     1) NMEA.ino works with your device (correct TX/RX pins and baud rate)
//     2) The RMC sentence has been enabled.
//     3) Your device sends an RMC sentence (e.g., $GPRMC).
//
//  'Serial' is for debug output to the Serial Monitor window.
//
//  License:
//    Copyright (C) 2014-2017, SlashDevin
//
//    This file is part of NeoGPS
//
//    NeoGPS is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NeoGPS is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with NeoGPS.  If not, see <http://www.gnu.org/licenses/>.
//
//======================================================================

#include <GPSport.h>

//------------------------------------------------------------
// Check that the config files are set up properly

#if !defined(NMEAGPS_PARSE_RMC)
#error You must uncomment NMEAGPS_PARSE_RMC in NMEAGPS_cfg.h!
#endif

#if !defined(GPS_FIX_TIME)
#error You must uncomment GPS_FIX_TIME in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_LOCATION)
#error You must uncomment GPS_FIX_LOCATION in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_SPEED)
#error You must uncomment GPS_FIX_SPEED in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_SATELLITES)
#error You must uncomment GPS_FIX_SATELLITES in GPSfix_cfg.h!
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
#error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

//------------------------------------------------------------

static NMEAGPS neoGPS; // This parses the GPS characters
namespace GPS
{

    //----------------------------------------------------------------
    //  Print the 32-bit integer degrees *as if* they were high-precision floats

    static void printL(Print &outs, int32_t degE7)
    {
        // Extract and print negative sign
        if (degE7 < 0)
        {
            degE7 = -degE7;
            outs.print('-');
        }

        // Whole degrees
        int32_t deg = degE7 / 10000000L;
        outs.print(deg);
        outs.print('.');

        // Get fractional degrees
        degE7 -= deg * 10000000L;

        // Print leading zeroes, if needed
        int32_t factor = 1000000L;
        while ((degE7 < factor) && (factor > 1L))
        {
            outs.print('0');
            factor /= 10L;
        }

        // Print fractional degrees
        outs.print(degE7);
    }
    static void displaySatellitesInView()
    {
        DEBUG_PORT.print(neoGPS.sat_count);
        DEBUG_PORT.print(',');

        for (uint8_t i = 0; i < neoGPS.sat_count; i++)
        {
            DEBUG_PORT.print(neoGPS.satellites[i].id);
            DEBUG_PORT.print(' ');
            DEBUG_PORT.print(neoGPS.satellites[i].elevation);
            DEBUG_PORT.print('/');
            DEBUG_PORT.print(neoGPS.satellites[i].azimuth);
            DEBUG_PORT.print('@');
            if (neoGPS.satellites[i].tracked)
                DEBUG_PORT.print(neoGPS.satellites[i].snr);
            else
                DEBUG_PORT.print('-');
            DEBUG_PORT.print(F(", "));
        }

        DEBUG_PORT.println();

    } // displaySatellitesInView
    static gps_fix doSomeWork(const gps_fix &fix, bikedata &data)
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
            data.satellites = fix.satellites;
            // data.updated = asctime(localtime(fix.dateTime));
            data.location = {fix.latitude(), fix.longitude(), fix.altitude_cm(), sat};
            data.speed_mph = fix.speed_mph();
        }

    } // doSomeWork

    //------------------------------------

    static void GPSloop(bikedata &data)
    {
        while (neoGPS.available(gpsPort))
            doSomeWork(neoGPS.read(), data);

    } // GPSloop

    //--------------------------

    void setup()
    {
        DEBUG_PORT.print(F("NMEAloc.INO: started\n"));
        DEBUG_PORT.print(F("fix object size = "));
        DEBUG_PORT.println(sizeof(neoGPS.fix()));
        DEBUG_PORT.print(F("NMEAGPS object size = "));
        DEBUG_PORT.println(sizeof(neoGPS));
        DEBUG_PORT.println(F("Looking for GPS device on " GPS_PORT_NAME));

#ifdef NMEAGPS_NO_MERGING
        DEBUG_PORT.println(F("Only displaying data from xxRMC sentences.\n  Other sentences may be parsed, but their data will not be displayed."));
#endif

        DEBUG_PORT.flush();

        gpsPort.begin(9600);
    }

    //--------------------------
}