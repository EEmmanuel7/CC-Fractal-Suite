/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Code related to everything related to Xforms in Fractal Space.
 */

#include <sys/SYS_Math.h>

#include "FractalSpace.h"

RSTORDER CC::get_rst_order(const int val)
{
	auto order = RSTORDER();

	// Houdini defines no more than 6 RST orders.
	if (val <= 5)
		order = static_cast<RSTORDER>(val);
	return order;
}

/// Calculates the 'tile origin', or the pixel number for where the tile 's first pixel is.
/// This is used as a reference point for the world pixel.
WORLDPIXELCOORDS CC::calculate_world_pixel(TIL_TileList* tiles, TIL_Tile* tile, int pixel_index)
{
	int tile_x, tile_y;
	// Note that these tile->getTileSizeX() sizes are generic,
	// while sizes from getSize() are correct
	tile->getSize(tile_x, tile_y);

	int x, y;
	// Base X pos for the tile, plus the X progression of the plane.
	x = tiles->myX1 + (pixel_index % tile_x);

	// Divide the pixel index into the size of x rounded down
	// For the tile's y, and add the base Y pos.
	y = SYSfloor((double)pixel_index / tile_x) + (tiles->myY1);
	return std::pair<int, int>(x, y);
}

/// Set Identity Matrix to matrix on construction.
CC::FractalSpace::FractalSpace()
{
	post_matrix.identity();
}

/// Constructs the matrix. from fed parameters
void CC::FractalSpace::set_xform(
	const double tx,
	const double ty,
	const double r,
	const double sx,
	const double sy,
	const double pivx,
	const double pivy,
	const RSTORDER xord = RSTORDER::TRS)
{
	// First xform without pivot, only translate and scale
	post_matrix.xform(xord, tx, ty, 0, sx, sy, 0, 0, 0);
	
	// Second xform with only rotate but with pivot set relative to screen space.
	double image_relative_y_size = image_y / image_x;

	// TODO: Make Y pivot relative to image size.

	post_matrix.xform(
		xord, 0, 0, r, 1, 1,
		post_matrix(2, 0) + (sx * pivx),
		post_matrix(2, 1) + (sy * pivy),
		0);

	rstorder = xord;
}


/// Return the fractal coordinates, which use the size of the image as a relative
/// Size. The scale is 0-1 in the x axis of the image.
FCOORDS
CC::FractalSpace::get_fractal_coords(WORLDPIXELCOORDS pixel_coords)
{
	FCOORDS fc{ pixel_coords.first / (double)image_x, pixel_coords.second / (double)image_x };
	UT_Matrix3 m;
	m.identity();
	m.xform(rstorder, fc.first, fc.second);

	m *= post_matrix;

	fc = { m(2, 0), m(2, 1) };  // xz, yz.

	return fc;
}

/// A fractal space must be populated with a default image size to be used by the context
/// Correctly.
/// The image size is used as a rest size for the fractal, which is normalized by
/// The size of the image. This is done so that changes in resolution don't require
/// Changes in transformation to see the same Fractal.
void CC::FractalSpace::set_image_size(int x, int y)
{
	image_x = x;
	image_y = y;
}

WORLDPIXELCOORDS CC::FractalSpace::get_image_size()
{
	return WORLDPIXELCOORDS(image_x, image_y);
}

/// Destructor
CC::FractalSpace::~FractalSpace() {}
