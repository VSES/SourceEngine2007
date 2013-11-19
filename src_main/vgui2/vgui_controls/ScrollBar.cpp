//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <assert.h>

#include <vgui/IScheme.h>
#include <vgui/ISystem.h>
#include <vgui/IInput.h>
#include <KeyValues.h>

#include <vgui_controls/ScrollBar.h>
#include <vgui_controls/ScrollBarSlider.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Controls.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

namespace
{

enum
{						 // scroll bar will scroll a little, then continuous scroll like in windows
	SCROLL_BAR_DELAY = 400,	 // default delay for all scroll bars
	SCROLL_BAR_SPEED = 50, // this is how fast the bar scrolls when you hold down the arrow button
	SCROLLBAR_DEFAULT_WIDTH = 17,
};

//-----------------------------------------------------------------------------
// Purpose: Scroll bar button-the arrow button that moves the slider up and down.
//-----------------------------------------------------------------------------
class ScrollBarButton : public Button
{
public:
	ScrollBarButton(Panel *parent, const char *panelName, const char *text) : Button(parent, panelName, text)
	{
		SetButtonActivationType(ACTIVATE_ONPRESSED);

		SetContentAlignment(Label::a_center);
	}

	void OnMouseFocusTicked()
	{
		// pass straight up to parent
		CallParentFunction(new KeyValues("MouseFocusTicked"));
	}
 
	virtual void ApplySchemeSettings(IScheme *pScheme)
	{
		Button::ApplySchemeSettings(pScheme);

		SetFont(pScheme->GetFont("Marlett", IsProportional() ));
		SetDefaultBorder(pScheme->GetBorder("ScrollBarButtonBorder"));
        SetDepressedBorder(pScheme->GetBorder("ScrollBarButtonDepressedBorder"));
		
		SetDefaultColor(GetSchemeColor("ScrollBarButton.FgColor", pScheme), GetSchemeColor("ScrollBarButton.BgColor", pScheme));
		SetArmedColor(GetSchemeColor("ScrollBarButton.ArmedFgColor", pScheme), GetSchemeColor("ScrollBarButton.ArmedBgColor", pScheme));
		SetDepressedColor(GetSchemeColor("ScrollBarButton.DepressedFgColor", pScheme), GetSchemeColor("ScrollBarButton.DepressedBgColor", pScheme));
	}

	// Don't request focus.
	// This will keep cursor focus in main window in text entry windows.
	virtual void OnMousePressed(MouseCode code)
	{
		if (!IsEnabled())
			return;
		
		if (!IsMouseClickEnabled(code))
			return;
		
		if (IsUseCaptureMouseEnabled())
		{
			{
				SetSelected(true);
				Repaint();
			}
			
			// lock mouse input to going to this button
			input()->SetMouseCapture(GetVPanel());
		}
	}
    virtual void OnMouseReleased(MouseCode code)
    {
		if (!IsEnabled())
			return;
		
		if (!IsMouseClickEnabled(code))
			return;
		
		if (IsUseCaptureMouseEnabled())
		{
			{
				SetSelected(false);
				Repaint();
			}
			
			// lock mouse input to going to this button
			input()->SetMouseCapture(NULL);
		}
    }

};

}

vgui::Panel *ScrollBar_Vertical_Factory()
{
	return new ScrollBar(NULL, NULL, true );
}

vgui::Panel *ScrollBar_Horizontal_Factory()
{
	return new ScrollBar(NULL, NULL, false );
}

DECLARE_BUILD_FACTORY_CUSTOM_ALIAS( ScrollBar, ScrollBar_Vertical, ScrollBar_Vertical_Factory );
DECLARE_BUILD_FACTORY_CUSTOM_ALIAS( ScrollBar, ScrollBar_Horizontal, ScrollBar_Horizontal_Factory );
// Default is a horizontal one
DECLARE_BUILD_FACTORY_CUSTOM( ScrollBar, ScrollBar_Horizontal_Factory );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
ScrollBar::ScrollBar(Panel *parent, const char *panelName, bool vertical) : Panel(parent, panelName)
{
	_slider=null;
	_button[0]=null;
	_button[1]=null;
	_scrollDelay = SCROLL_BAR_DELAY;
	_respond = true;

	if (vertical)
	{
		// FIXME: proportional changes needed???
		SetSlider(new ScrollBarSlider(NULL, NULL, true));
		SetButton(new ScrollBarButton(NULL, NULL, "t"), 0);
		SetButton(new ScrollBarButton(NULL, NULL, "u"), 1);
		_button[0]->SetTextInset(0, 1);
		_button[1]->SetTextInset(0, -1);

		SetSize(SCROLLBAR_DEFAULT_WIDTH, 64);
	}
	else
	{
		SetSlider(new ScrollBarSlider(NULL, NULL, false));
		SetButton(new ScrollBarButton(NULL, NULL, "w"), 0);
		SetButton(new ScrollBarButton(NULL, NULL, "4"), 1);
		_button[0]->SetTextInset(0, 0);
		_button[1]->SetTextInset(0, 0);

		SetSize(64, SCROLLBAR_DEFAULT_WIDTH);
	}

	Panel::SetPaintBorderEnabled(true);
	Panel::SetPaintBackgroundEnabled(false);
	Panel::SetPaintEnabled(true);
	SetButtonPressedScrollValue(20);
	SetBlockDragChaining( true );

	Validate();
}

//-----------------------------------------------------------------------------
// Purpose: sets up the width of the scrollbar according to the scheme
//-----------------------------------------------------------------------------
void ScrollBar::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	const char *resourceString = pScheme->GetResourceString("ScrollBar.Wide");

	if (resourceString)
	{
		int value = atoi(resourceString);
		if (IsProportional())
		{
			value = scheme()->GetProportionalScaledValueEx(GetScheme(), value);
		}

		if (_slider && _slider->IsVertical())
		{
			// we're vertical, so reset the width
			SetSize( value, GetTall() );
		}
		else
		{
			// we're horizontal, so the width means the height
			SetSize( GetWide(), value );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Set the slider's Paint border enabled.
//-----------------------------------------------------------------------------
void ScrollBar::SetPaintBorderEnabled(bool state)
{
	if ( _slider )
	{
		_slider->SetPaintBorderEnabled( state );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ScrollBar::SetPaintBackgroundEnabled(bool state)
{
	if ( _slider )
	{
		_slider->SetPaintBackgroundEnabled( state );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ScrollBar::SetPaintEnabled(bool state)
{
	if ( _slider )
	{
		_slider->SetPaintEnabled( state );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Layout the scroll bar and buttons on screen
//-----------------------------------------------------------------------------
void ScrollBar::PerformLayout()
{
	if (_slider)
	{
		int wide, tall;
		GetPaintSize(wide,tall);
		if(_slider->IsVertical())
		{
			_slider->SetBounds(0, wide, wide, tall-(wide*2)+1);
			_button[0]->SetBounds(0,0, wide-1, wide );
			_button[1]->SetBounds(0,tall-wide ,wide-1, wide );
		}
		else
		{
			_slider->SetBounds(tall, -1, wide-(tall*2)+1, tall + 1 );
			_button[0]->SetBounds(0, 0, tall, tall);
			_button[1]->SetBounds(wide-tall, 0, tall, tall);
		}
		// after resizing our child, we should remind it to perform a layout
		_slider->InvalidateLayout();
	}
	// get tooltips to draw
	Panel::PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Set the value of the scroll bar slider.
//-----------------------------------------------------------------------------
void ScrollBar::SetValue(int value)
{
	_slider->SetValue(value);
}

//-----------------------------------------------------------------------------
// Purpose: Get the value of the scroll bar slider.
//-----------------------------------------------------------------------------
int ScrollBar::GetValue()
{
	return _slider->GetValue();
}

//-----------------------------------------------------------------------------
// Purpose: Set the range of the scroll bar slider.
// This the range of numbers the slider can scroll through.
//-----------------------------------------------------------------------------
void ScrollBar::SetRange(int min,int max)
{
	_slider->SetRange(min,max);
}

//-----------------------------------------------------------------------------
// Purpose: Gets the range of the scroll bar slider.
// This the range of numbers the slider can scroll through.
//-----------------------------------------------------------------------------
void ScrollBar::GetRange(int &min, int &max)
{
    _slider->GetRange(min, max);
}

//-----------------------------------------------------------------------------
// Purpose: Send a message when the slider is moved.
// Input  : value - 
//-----------------------------------------------------------------------------
void ScrollBar::SendSliderMoveMessage(int value)
{
	PostActionSignal(new KeyValues("ScrollBarSliderMoved", "position", value));
}

//-----------------------------------------------------------------------------
// Purpose: Called when the Slider is dragged by the user
// Input  : value - 
//-----------------------------------------------------------------------------
void ScrollBar::OnSliderMoved(int value)
{
	SendSliderMoveMessage(value);
}

//-----------------------------------------------------------------------------
// Purpose: Check if the scrollbar is vertical (true) or horizontal (false)
//-----------------------------------------------------------------------------
bool ScrollBar::IsVertical()
{
	return _slider->IsVertical();
}

//-----------------------------------------------------------------------------
// Purpose: Check if the the scrollbar slider has full range.
// Normally if you have a scroll bar and the range goes from a to b and
// the slider is sized to c, the range will go from a to b-c.
// This makes it so the slider goes from a to b fully.
//-----------------------------------------------------------------------------
bool ScrollBar::HasFullRange()
{
	return _slider->HasFullRange();
}

//-----------------------------------------------------------------------------
// Purpose: Setup the indexed scroll bar button with the input params.
//-----------------------------------------------------------------------------
//LEAK: new and old slider will leak
void ScrollBar::SetButton(Button *button, int index)
{
	if(_button[index]!=null)
	{
		_button[index]->SetParent((Panel *)NULL);
	}
	_button[index]=button;
	_button[index]->SetParent(this);
	_button[index]->AddActionSignalTarget(this);
	_button[index]->SetCommand(new KeyValues("ScrollButtonPressed", "index", index));

	Validate();
}


//-----------------------------------------------------------------------------
// Purpose: Return the indexed scroll bar button
//-----------------------------------------------------------------------------
Button* ScrollBar::GetButton(int index)
{
	return _button[index];
}


//-----------------------------------------------------------------------------
// Purpose: Set up the slider.
//-----------------------------------------------------------------------------
//LEAK: new and old slider will leak
void ScrollBar::SetSlider(ScrollBarSlider *slider)
{
	if(_slider!=null)
	{
		_slider->SetParent((Panel *)NULL);
	}
	_slider=slider;
	_slider->AddActionSignalTarget(this);
	_slider->SetParent(this);

	Validate();
}

//-----------------------------------------------------------------------------
// Purpose: Return a pointer to the slider.
//-----------------------------------------------------------------------------
ScrollBarSlider *ScrollBar::GetSlider()
{
	return _slider;
}

//-----------------------------------------------------------------------------
// Purpose: Scrolls in response to clicking and holding on up or down arrow
// The idea is to have the slider move one step then delay a bit and then
// the bar starts moving at normal speed. This gives a stepping feeling
// to just clicking an arrow once.
//-----------------------------------------------------------------------------
void ScrollBar::OnMouseFocusTicked()
{
	int direction = 0;
	
	// top button is down
	if ( _button[0]->IsDepressed() )
	{
		direction = -1;
	}
	// bottom top button is down
	else if (_button[1]->IsDepressed())
	{
		direction = 1;
	}

	// a button is down 
	if ( direction != 0 )  
	{
		RespondToScrollArrow(direction);
		if (_scrollDelay < system()->GetTimeMillis())
		{
			_scrollDelay = system()->GetTimeMillis() + SCROLL_BAR_SPEED;
			_respond = true; 
		}
		else
		{
			_respond = false; 
		}		
	}
	// a button is not down.
	else 
	{
		// if neither button is down keep delay at max
		_scrollDelay = system()->GetTimeMillis() + SCROLL_BAR_DELAY;
		_respond = true; 
	}
}

//-----------------------------------------------------------------------------
// Purpose: move scroll bar in response to the first button
// Input: button and direction to move scroll bar when that button is pressed
//			direction can only by +/- 1 
// Output: whether button is down or not
//-----------------------------------------------------------------------------
void ScrollBar::RespondToScrollArrow(int const direction)
{
	if (_respond)
	{
		int newValue = _slider->GetValue() + (direction * _buttonPressedScrollValue);
		_slider->SetValue(newValue);
		SendSliderMoveMessage(newValue);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Trigger layout changes when the window size is changed.
//-----------------------------------------------------------------------------
void ScrollBar::OnSizeChanged(int wide, int tall)
{
	InvalidateLayout();
	_slider->InvalidateLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Set how far the scroll bar slider moves when a scroll bar button is
// pressed.
//-----------------------------------------------------------------------------
void ScrollBar::SetButtonPressedScrollValue(int value)
{
	_buttonPressedScrollValue=value;
}

//-----------------------------------------------------------------------------
// Purpose: Set the range of the rangewindow. This is how many 
// lines are displayed at one time 
// in the window the scroll bar is attached to.
// This also controls the size of the slider, its size is proportional
// to the number of lines displayed / total number of lines.
//-----------------------------------------------------------------------------
void ScrollBar::SetRangeWindow(int rangeWindow)
{
	_slider->SetRangeWindow(rangeWindow);
}

//-----------------------------------------------------------------------------
// Purpose: Get the range of the rangewindow. This is how many 
// lines are displayed at one time 
// in the window the scroll bar is attached to.
// This also controls the size of the slider, its size is proportional
// to the number of lines displayed / total number of lines.
//-----------------------------------------------------------------------------
int ScrollBar::GetRangeWindow()
{
	return _slider->GetRangeWindow();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void ScrollBar::Validate()
{
	if ( _slider != null )
	{
		int buttonOffset = 0;

		for( int i=0; i<2; i++ )
		{
			if( _button[i] != null )
			{
				if( _button[i]->IsVisible() )
				{
					if( _slider->IsVertical() )
					{					
						buttonOffset += _button[i]->GetTall();
					}
					else
					{
						buttonOffset += _button[i]->GetWide();
					}
				}
			}
		}

		_slider->SetButtonOffset(buttonOffset);
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void ScrollBar::SetScrollbarButtonsVisible(bool visible)
{
	for( int i=0; i<2; i++ )
	{
		if( _button[i] != null )
		{
			_button[i]->SetShouldPaint( visible );
		}
	}
}
