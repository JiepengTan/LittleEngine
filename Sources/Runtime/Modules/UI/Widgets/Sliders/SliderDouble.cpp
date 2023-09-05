/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Modules/UI/Widgets/Sliders/SliderDouble.h"

LittleEngine::UI::Widgets::Sliders::SliderDouble::SliderDouble
(
	double p_min,
	double p_max,
	double p_value,
	ESliderOrientation p_orientation,
	const std::string & p_label,
	const std::string & p_format
) : SliderSingleScalar<double>(ImGuiDataType_::ImGuiDataType_Double, p_min, p_max, p_value, p_orientation, p_label, p_format)
{
}
