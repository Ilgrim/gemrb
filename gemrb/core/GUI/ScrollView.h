/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2016 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef __GemRB__ScrollView__
#define __GemRB__ScrollView__

#include "GUI/GUIAnimation.h"
#include "GUI/View.h"

namespace GemRB {
	class ScrollBar;

	class GEM_EXPORT ScrollView : public View {

		class ContentView : public View {
			private:
			void SizeChanged(const Size& /* oldsize */);
			
			void SubviewAdded(View* view, View* parent);
			void SubviewRemoved(View* view, View* parent);
			
			void ResizeToSubviews();
			
			public:
			ContentView(const Region& frame)
			: View(frame) {}
			bool CanLockFocus() const { return false; }
		};
		
		PointAnimation animation;

		ScrollBar* hscroll;
		ScrollBar* vscroll;

		ContentView contentView;

	private:
		void ContentFrameChanged();
		void ScrollbarValueChange(ScrollBar*);
		
		void WillDraw();
		void DidDraw();
		
		void FlagsChanged(unsigned int /*oldflags*/);

	public:
		ScrollView(const Region& frame);
		~ScrollView();

		void AddSubviewInFrontOfView(View*, const View* = NULL);
		View* RemoveSubview(const View*);
		View* SubviewAt(const Point&, bool ignoreTransparency = false, bool recursive = false);

		bool OnKeyPress(const KeyboardEvent& /*Key*/, unsigned short /*Mod*/);
		void OnMouseWheelScroll(const Point& delta);
		void OnMouseDrag(const MouseEvent&);
		
		Point ScrollOffset() const;
		void SetScrollIncrement(int);
		void ScrollDelta(const Point& p, ieDword duration = 0);
		void ScrollTo(Point p, ieDword duration = 0);
	};
}
#endif /* __GemRB__ScrollView__ */