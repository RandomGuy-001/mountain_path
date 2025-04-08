
#include "Bridges.h"
#include "ColorGrid.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath> 

#include "DataSource.h"
#include "data_src/ElevationData.h"
using namespace std;
using namespace bridges;

// This program implements the mountain paths assignment in C++. See the README.md for a
// detailed description


// takes in the processed elevation data and returns a color grid for
// visualization using BRIDGES
ColorGrid getImage(const ElevationData& elev_data) {
  // Get the min and max elevation for given area
  int min_elev = elev_data.getMinVal();
  int max_elev = elev_data.getMaxVal();

  // Get length and width of elev_data
  int cols = elev_data.getCols();
  int rows = elev_data.getRows();
  
  // Create color Grid that has same width and length of elev_data and default color 0
  ColorGrid cg(cols, rows, Color(0, 0, 0));

  // Nested for loop to set color for each cell corisponding with elev_data value mapped to 0-255
  for (int r = 0; r < rows; r++){
  	for (int c = 0; c < cols; c++){
		
		// get the elevation for given row/col position
		int elevation = elev_data.getVal(r, c);
		// calculate elevation scaled to 0-255
		int scaled = ((elevation - min_elev) * 255) / (max_elev - min_elev);

		// set color at row/col position to scaled value
		cg.set(r, c, Color(scaled, scaled, scaled));
		}
	}  
  return cg;
}

// determines the least effort path through the mountain starting a point on
// the left edge of the image
void findPath(const ElevationData&  elev_data, int startRow, ColorGrid& cg) {
  
  int rows = cg.getHeight();
  int cols = cg.getWidth();
  int currentCol = 0;
  int currentRow = startRow;
  int upElev = 0;
  int middleElev = 0;
  int downElev = 0;
  cg.set(startRow, 0, Color(255, 0, 0));
  // Debug, print rows and cols
  std::cout << "Rows: " << rows << ", Columns: " << cols << std::endl;
  while (currentCol < cols - 1){
    // this section gets the elevations of each possible next move
    int currentElev = elev_data.getVal(currentRow, currentCol);
	int middleElev = elev_data.getVal(currentRow,  (currentCol + 1));
	// check that if current row is already at top of map
	int upElev = elev_data.getVal((currentRow - 1), (currentCol + 1));
	int downElev = elev_data.getVal((currentRow + 1), (currentCol + 1));
	// This section calculates the differences between each possible spot and current spot
	int middleDiff = abs(currentElev - middleElev);
	int upDiff = abs(currentElev - upElev);
	int downDiff = abs(currentElev - downElev); 

    // Dang ol' if/else man
	if ((middleDiff < upDiff) and (middleDiff < downDiff)){
		currentRow = currentRow;
	}
	else if ((upDiff < middleDiff) and (upDiff < downDiff)){
		currentRow -= 1;  
	}
	else if ((downDiff < middleDiff) and (downDiff < upDiff)){
		currentRow +=1;
	}
    std::cout << "Step (Current Column): " << currentCol
          << " | Current Elevation: " << currentElev
          << " | Middle Elevation: " << middleElev
          << " | Up Elevation: " << upElev
          << " | Down Elevation: " << downElev << std::endl;

	currentCol += 1;
	cg.set(currentRow, currentCol, Color(255, 0, 0));
	}
}


int main(int argc, char **argv) {

  // bridges object initialization


  // initialize Bridges
  Bridges bridges(1, "tessjtalbot", "1729671476901");

  // defaults for row number and data file
  int startRow = 50;

  // set title
  bridges.setTitle("Mountain Paths - Greedy Algorithms Example");
  bridges.setDescription("Your goal is to find a path that takes you through the points with the lowest elevation changes, in an effort to minimize the overall effort in walking through the path.");

  // get elevation data

  DataSource ds;
  ElevationData elev_data = ds.getElevationData(6.02, 44.10, 9.70, 47.77, 0.02);

  // color grid for visualization
  ColorGrid cg = getImage(elev_data);

  // find path by applying a greedy algorithm
  startRow = elev_data.getRows()/2;
  findPath (elev_data, startRow, cg);

  // visualize
  bridges.setDataStructure(cg);
  bridges.visualize();

  return 0;
}
