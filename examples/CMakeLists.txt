#  Copyright (C) 2015 Enrico Mariotti <enricomariotti@yahoo.it>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 or (at your option)
#  version 3 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

configure_file(empty.csv u_empty.csv NEWLINE_STYLE UNIX)
configure_file(empty.csv d_empty.csv NEWLINE_STYLE DOS)
configure_file(onerow.csv u_onerow.csv NEWLINE_STYLE UNIX)
configure_file(onerow.csv d_onerow.csv NEWLINE_STYLE DOS)
configure_file(onefield.csv u_onefield.csv NEWLINE_STYLE UNIX)
configure_file(onefield.csv d_onefield.csv NEWLINE_STYLE DOS)
configure_file(emptylines.csv u_emptylines.csv NEWLINE_STYLE UNIX)
configure_file(emptylines.csv d_emptylines.csv NEWLINE_STYLE DOS)
configure_file(corner.csv u_corner.csv NEWLINE_STYLE UNIX)
configure_file(corner.csv d_corner.csv NEWLINE_STYLE DOS)
configure_file(diffcolumns.csv u_diffcolumns.csv NEWLINE_STYLE UNIX)
configure_file(diffcolumns.csv d_diffcolumns.csv NEWLINE_STYLE DOS)
configure_file(sol049.csv u_sol049.csv NEWLINE_STYLE UNIX)
configure_file(sol049.csv d_sol049.csv NEWLINE_STYLE DOS)
