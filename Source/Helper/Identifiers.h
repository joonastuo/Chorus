/*
  ==============================================================================

    Identifiers.h
    Created: 25 Mar 2019 12:02:18pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

namespace IDs {
	// JUCE Strings
	#define DECLARE_ID(name) const juce::String name (#name);

	#undef DECLARE_ID
}