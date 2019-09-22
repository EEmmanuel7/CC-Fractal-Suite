/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Code related to everything related to Xforms in Fractal Space.
 */

#include <sys/SYS_Math.h>

#include "FractalSpace.h"

using namespace CC;

RSTORDER CC::get_rst_order(const int val)
{
	auto order = RSTORDER();

	// Houdini defines no more than 6 RST orders.
	if (val <= 5)
		order = static_cast<RSTORDER>(val);
	return order;
}

/// Calculates the minimum and maximum pixel positions for a given tile.
void CC::calculate_tile_minmax(
	TIL_Tile* tile,
	WORLDPIXELCOORDS& min,
	WORLDPIXELCOORDS& max)
{
	int offset_x, offset_y;
	tile->getOffset(offset_x, offset_y);

	int size_x, size_y;
	tile->getSize(size_x, size_y);

	min = WORLDPIXELCOORDS(offset_x, offset_y);

	max = WORLDPIXELCOORDS(
		offset_x + size_x - 1,
		offset_y + size_y - 1);

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
FractalSpace::FractalSpace()
{
	post_matrix.identity();
}

FractalSpace::FractalSpace(int x, int y)
{
	set_image_size(x, y);
}


void FractalSpace::set_xform(MultiXformStashData & xdata)
{
	UT_Matrix3D m;
	m.identity();

	auto xforms = xdata.xforms;

	// Set ident matrix is xdata vector is zero.
	if (xforms.size() < 1)
		post_matrix = m;

	fpreal xyRatio = (image_y / (fpreal)image_x);
	fpreal px, py;
	px = py = 0.0;

	UT_Matrix3D m_pivXform{ 1, 0, 0, 0, 1, 0, 0.5, 0.5 * xyRatio, 1 };
	UT_Matrix3D m_pivIterated, m_local;
	fpreal overall_scale = 1.0;
	for (int i = 0; i < xforms.size(); ++i)
	{
		// Create center pivot from xform space.
		m_pivIterated = m_pivXform * m;
		px = m_pivIterated(2, 0);
		py = m_pivIterated(2, 1);

		// Store overall scale. This will be applied to
		// The offsets, so that their transforms are local
		// and are accounting previous transformations.
		overall_scale *= xforms[i].scale;

		// Get the local transform
		m_local.identity();
		m_local.xform(
			xforms[i].xord,
			xforms[i].offset_x * overall_scale,
			xforms[i].offset_y * overall_scale,
			xforms[i].rotate,
			xforms[i].scale, xforms[i].scale,
			px, py);

		// Add the local transform to our transform chain.
		m *= m_local;
	}

	// Apply our resolved xform to the node's viewer xform
	post_matrix = m;
}

void FractalSpace::set_xform(XformStashData& xformData)
{
	xdata = xformData;

	post_matrix.xform(
		xdata.xord,
		xdata.offset_x, xdata.offset_y,
		xdata.rotate,
		xdata.scale, xdata.scale);
}

void FractalSpace::set_xform(UT_Matrix3D & xform)
{
	post_matrix = xform;
}

/// Return the fractal coordinates, which use the size of the image as a relative
/// Size. The scale is 0-1 in the x axis of the image.
COMPLEX
FractalSpace::get_fractal_coords(WORLDPIXELCOORDS pixel_coords)
{
	COMPLEX fc{ pixel_coords.first / (double)image_x, pixel_coords.second / (double)image_x };
	UT_Matrix3D m;
	m.identity();
	m.xform(xdata.xord, fc.real(), fc.imag());

	m *= post_matrix;

	fc = { m(2, 0), m(2, 1) };  // xz, yz.

	return fc;
}

// TODO: Move the body of this code and the other version to remove code duplication
COMPLEX FractalSpace::get_fractal_coords(COMPLEX pixel_coords)
{
	COMPLEX fc{ pixel_coords.real() / (double)image_x, pixel_coords.imag() / (double)image_x };
	UT_Matrix3D m;
	m.identity();
	m.xform(xdata.xord, fc.real(), fc.imag());

	m *= post_matrix;

	fc = { m(2, 0), m(2, 1) };  // xz, yz.

	return fc;
}

/// Return the world coords from source
WORLDPIXELCOORDS
FractalSpace::get_pixel_coords(COMPLEX fractal_coords)
{
	UT_Matrix3D m;
	m.identity();

	// Set m to fractal coordinates
	m.xform(xdata.xord, fractal_coords.real(), fractal_coords.imag());

	// Apply our object's transform
	m.xform(
		xdata.xord,
		xdata.offset_x, xdata.offset_y,
		xdata.rotate,
		xdata.scale, xdata.scale * (image_y / (double)image_x),
		0, 0,
		true);

	int x = static_cast<int>(m(2, 0) * image_x);
	int y = static_cast<int>(m(2, 1) * image_y);

	return WORLDPIXELCOORDS(x, y);
}

COMPLEX FractalSpace::get_minimum()
{
	return get_fractal_coords(WORLDPIXELCOORDS(0, 0));
}

COMPLEX FractalSpace::get_maximum()
{
	return get_fractal_coords(get_image_size());
}

/// A fractal space must be populated with a default image size to be used by the context
/// Correctly.
/// The image size is used as a rest size for the fractal, which is normalized by
/// The size of the image. This is done so that changes in resolution don't require
/// Changes in transformation to see the same Fractal.
void FractalSpace::set_image_size(int x, int y)
{
	image_x = x;
	image_y = y;
}

WORLDPIXELCOORDS FractalSpace::get_image_size()
{
	return WORLDPIXELCOORDS(image_x, image_y);
}

/// Destructor
FractalSpace::~FractalSpace() {}
