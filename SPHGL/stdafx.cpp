#include "stdafx.h"

bool Param::ctrlParam[10];
Param::Param()
{
}

Param::~Param()
{
}

void Param::initParam()
{
	ctrlParam[IS_SURFACE] = false;
	ctrlParam[IS_DIS_FIELD] = false;
	ctrlParam[IS_PARTICLE] = true;
	ctrlParam[IS_HASH_GRID] = false;
	ctrlParam[IS_OUTPUT_FRAME] = false;
}