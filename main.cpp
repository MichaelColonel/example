/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <iostream>

#include <trec_system_configure.hh>
#include <trec_hits_positions.hh>
#include <trec_track_coordinates.hh>
#include <trec_tracks_reconstruction.hh>

int
main(void)
{
	TREC::HitsPositionsVector positions;
	TREC::MainTracksVector mains;
	TREC::FullTracksVector fulls;
	TREC::HitsPositions::load( "hits.dat", positions);

	TREC::SharedConf conf = TREC::SystemConfigure::instance("file.conf");

	for ( TREC::HitsPositionsVector::const_iterator iter = positions.begin();
		iter != positions.end(); ++iter)
	{
		bool main_flag, full_flag; // main track, full track flags
		TREC::TrackCoordinates track_coord(*iter);
		track_coord.calculate_tracks( main_flag, full_flag);

		if (full_flag) {
			// main track, full track
			TREC::TrackXYPair main_track, full_track;
			track_coord.get_tracks( main_track, full_track);

			// calorimeter position
			int cpos = iter->calorimeter_position();
			TREC::TracksPositionPair track( full_track, cpos);
		
			mains.push_back(main_track);
			fulls.push_back(track);
		}
	}

	int binx = 60; // histogram bins along X-axis
	int biny = 60; // histogram bins along Y-axis
	TREC::TracksReconstruction rec( mains, fulls,
		-3.0 * CLHEP::cm, 3.0 * CLHEP::cm,
		-3.0 * CLHEP::cm, 3.0 * CLHEP::cm,
		binx, biny);

	int slice_obj_min = 150; // min slice number of calorimeter, object exposure
	int slice_obj_max = 213; // max slice number of calorimeter, object exposure
	int slice_clr_max = 213; // max slice number of calorimeter, Bragg Peak

	rec.reconstruct( slice_obj_min, slice_obj_max, slice_clr_max);
	rec.save("test.root"); 

	return 0;
}
