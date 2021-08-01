#-----------------------------------------------------------------------------
# This file is part of Twitch Controls Chaos (TCC).
# Copyright 2021 blegas78
#
# TCC is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# TCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with TCC.  If not, see <https://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------------

from flexx import flx
from matplotlib.colors import is_color_like

class ColorProgressBar(flx.ProgressBar):

    def init(self, bar_color="#8be"):
        super().init()
        set_color(col)
        
    def set_color(col):
        if is_color_like(col):
            bar_color = col

    def _render_dom(self):
        global Math
        value = self.value
        mi, ma = self.min, self.max
        perc = 100 * (value - mi) / (ma - mi)
        label = self.text
        label = label.replace('{value}', str(value))
        label = label.replace('{percent}', Math.round(perc) + '%')
        attr = {'style__width': perc+'%',
                'style__height': '100%',
                'className': 'progress-bar',
                'style.background': bar_color,
                }
        return [flx.create_element('div', attr, label)]
