/*
	Cole Clark's Fractal Suite

	FractalNode.h
	Header defining shared fundamentals of Fractal Nodes.
	Note that this file is header-only.
 */

#pragma once

#include <string>

#include <PRM/PRM_Include.h>
#include <PRM/PRM_ChoiceList.h>
#include <OP/OP_Network.h>
#include <CH/CH_Manager.h>
#include <sstream>
#include <iomanip>

#include "typedefs.h"

/// Fractal Xform Name Data

static PRM_Name nameScale(SCALE_NAME.first, SCALE_NAME.second);
static PRM_Name nameOffset(TRANSLATE_NAME.first, TRANSLATE_NAME.second);
static PRM_Name nameRotate(ROTATE_NAME.first, ROTATE_NAME.second);
static PRM_Name nameXOrd(XORD_NAME.first, XORD_NAME.second);

/// Fractal Xform Multi Name Data
static PRM_Name nameXforms{ XFORMS_NAME.first, XFORMS_NAME.second };
static PRM_Name nameMultiScale(SCALE_M_NAME.first, SCALE_M_NAME.second);
static PRM_Name nameMultiOffset(TRANSLATE_M_NAME.first, TRANSLATE_M_NAME.second);
static PRM_Name nameMultiRotate(ROTATE_M_NAME.first, ROTATE_M_NAME.second);
static PRM_Name nameMultiXOrd(XORD_M_NAME.first, XORD_M_NAME.second);

/// Mandelbrot Name  Data
static PRM_Name nameIter{ ITERS_NAME.first, ITERS_NAME.second };
static PRM_Name namePower{ POWER_NAME.first, POWER_NAME.second };
static PRM_Name nameBailout{ BAILOUT_NAME.first, BAILOUT_NAME.second };
static PRM_Name nameJDepth{ JDEPTH_NAME.first, JDEPTH_NAME.second };
static PRM_Name nameJOffset{ JOFFSET_NAME.first, JOFFSET_NAME.second };
static PRM_Name nameBlackhole{ BLACKHOLE_NAME.first, BLACKHOLE_NAME.second };

/// Declare Lyapunov Parm Names
static PRM_Name nameLyaSeq(LYASEQ_NAME.first, LYASEQ_NAME.second);
static PRM_Name nameLyaSeqWeights(LYASEQWEIGHTS_NAME.first, LYASEQWEIGHTS_NAME.second);
static PRM_Name nameLyaStart(LYASTART_NAME.first, LYASTART_NAME.second);
static PRM_Name nameLyaMaxValue(LYACEILVALUE_NAME.first, LYACEILVALUE_NAME.second);
static PRM_Name nameLyaInvertNegative(LYAINVERTNEGATIVE_NAME.first, LYAINVERTNEGATIVE_NAME.second);


/// ChoiceList Lists
static PRM_Name xordMenuNames[] =
{
	PRM_Name("TRS", "Translate Rotate Scale"),
	PRM_Name("TSR", "Translate Scale Rotate"),
	PRM_Name("RTS", "Rotate Translate Scale"),
	PRM_Name("RST", "Rotate Scale Translate"),
	PRM_Name("STR", "Scale Translate Rotate"),
	PRM_Name("SRT", "Scale Rotate Translate"),
	PRM_Name(0)
};

static PRM_ChoiceList xOrdMenu
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	::xordMenuNames
);

/// Xform Defaults Data
/// The Y value is equal to the default rez coefficient * 50%.
/// Literally, 1080 / 1920 * 0.5
static PRM_Default defaultScale{ 5 };
static PRM_Default defaultOffset[] = { -0.6, -0.28125 };
static PRM_Default defaultRotate{ 0.0f };
static PRM_Default defaultXOrd{ RSTORDER::TRS };
static PRM_Default defaultMultiXOrd{ RSTORDER::RST };

static PRM_Default defaultOffsetBuddhabrot[] = { -3.0, -1.3 };

/// Mandelbrot Defaults Data
static PRM_Default defaultIter{ 50 };
static PRM_Default defaultPower{ 2 };
static PRM_Default defaultBailout{ 4 };  // 4 Looks good at 4k when smoothing.
static PRM_Default defaultJDepth{ 0 };
static PRM_Default defaultJOffset[] = { 0, 0 };
static PRM_Default defaultBlackhole{ false };

/// Declare Lyapunov Defaults
static PRM_Default defaultLyaMaxValue(5.0f);
static PRM_Default defaultLyaStart(0.5);

/// Xform Parm Ranges
static PRM_Range rangeScale
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, defaultScale.getFloat()
};

static PRM_Range rangeRotate
{
	PRM_RangeFlag::PRM_RANGE_UI, -180,
	PRM_RangeFlag::PRM_RANGE_UI, 180
};

/// Mandelbrot Parm Ranges
static PRM_Range rangeIter
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 200
};


static PRM_Range rangePower
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 10
};

static PRM_Range rangeBailout
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 4
};

static PRM_Range rangeJDepth
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 5
};

static PRM_Range rangeLyaStartValue
{
	PRM_RangeFlag::PRM_RANGE_UI, 0.001f,
	PRM_RangeFlag::PRM_RANGE_UI, 0.999f
};

static PRM_Range rangeLyaMaxValue
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, defaultLyaMaxValue.getFloat()
};

static PRM_Range rangeLyaSeq
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 40
};

static PRM_Range rangeLyaIters
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 40
};

/// Multiparm Templates
static PRM_Template multiparmSeqTemps[] =
{
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameLyaSeqWeights, PRMzeroDefaults),
	PRM_Template()
};

static PRM_Template multiparmXformTemps[] =
{
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMultiXOrd, &defaultMultiXOrd, &xOrdMenu),
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameMultiScale, PRMoneDefaults, 0, &rangeScale),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameMultiOffset, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameMultiRotate, PRMzeroDefaults, 0, &rangeRotate),
	PRM_Template()
};

/// Create separator names.
static PRM_Name nameSeparatorMandelbrot("sep_mandelbrot", "Sep Mandelbrot");
static PRM_Name nameSepA("sep_A", "Sep A");
static PRM_Name nameSepB("sep_B", "Sep B");
static PRM_Name nameSepC("sep_C", "Sep C");

/// Definition of Template Switcher
#define TEMPLATE_SWITCHER PRM_Template(PRM_SWITCHER, 4, &PRMswitcherName, switcher)

/// Definition of Xform Templates for myTemplateList calls, add 4 to COP_SWITCHER calls
#define TEMPLATES_XFORM \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameXOrd, &defaultXOrd, &xOrdMenu), \
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, defaultOffset), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameRotate, PRMzeroDefaults, 0, &rangeRotate)

#define TEMPLATES_XFORM_MULTI \
	PRM_Template(PRM_MULTITYPE_LIST, multiparmXformTemps, 1, &nameXforms, PRMzeroDefaults)


#define TEMPLATES_XFORM_BUDDHABROT \
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, defaultOffsetBuddhabrot), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameRotate, PRMzeroDefaults, 0, &rangeRotate)

/// Definition of Mandelbrot Templates. Add 7 to COP_SWITCHER calls
#define TEMPLATES_MANDELBROT \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeIter), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePower, &defaultPower, 0, &rangePower), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameBailout, &defaultBailout, 0, &rangeBailout), \
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameBlackhole, PRMzeroDefaults), \
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSeparatorMandelbrot, PRMzeroDefaults), \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameJDepth, PRMzeroDefaults, 0, &rangeJDepth), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameJOffset, PRMzeroDefaults)

/// Definition of Lyapunov Templates. Add 5 to COP_SWITCHER calls
#define TEMPLATES_LYAPUNOV \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeLyaIters), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameLyaStart, &defaultLyaStart, 0, &rangeLyaStartValue), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameLyaMaxValue, &defaultLyaMaxValue, 0, &rangeLyaMaxValue), \
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameLyaInvertNegative, PRMoneDefaults), \
	PRM_Template(PRM_MULTITYPE_LIST, multiparmSeqTemps, 1, &nameLyaSeq, PRMoneDefaults, &rangeLyaSeq)


namespace CC
{
	/// Boilerplate code calling the constructor for newCop2Operator.
	/// Do not use this if you need to modify the class constructor.
	/// This is only meant for vanilla calls.
	/// I read online that template calls like this must be declared
	/// in the header, which is why it's here.
	class OP
	{
	public:
		template <typename T>
		static OP_Node* get_new_op(
			OP_Network* net,
			const char* name,
			OP_Operator* op)
		{
			return new T(net, name, op);
		}
	};

	struct MultiXformData
	{
		fpreal scale{ 1.0 };
		COMPLEX offset{ 0.0, 0.0 };
		fpreal rotate{ 0.0 };
	};


	/// Takes a node with TEMPLATES_XFORM_MULTI defined, and return
	/// The overall requested offset and scale.
	static MultiXformData
	get_multi_xform_sums(OP_Node* node)
	{
		MultiXformData data;

		fpreal t = CHgetEvalTime();

		int numXforms = node->evalInt(XFORMS_NAME.first, 0, t);
		for (int i = 1; i <= numXforms; ++i)
		{
			data.scale *= node->evalFloatInst(
				SCALE_M_NAME.first, &i, 0, t);

			data.offset += (
				node->evalFloatInst(TRANSLATE_M_NAME.first, &i, 0, t),
				node->evalFloatInst(TRANSLATE_M_NAME.first, &i, 1, t));

			data.rotate += node->evalFloatInst(
				ROTATE_M_NAME.first, &i, 0, t);
		}

		return data;
	};

	static std::string
	formatMultiXformData(MultiXformData data)
	{
		std::ostringstream oss;

		oss << std::setprecision(2);
		oss << "Fractal Transformation:" << std::endl;
		oss << "Scale: " << data.scale << std::endl;
		oss << "Transform: " << data.offset.real() << " "
			<< data.offset.imag() << std::endl;
		oss << "Rotate: " << data.rotate << std::endl;
		
		return oss.str();
	}
}