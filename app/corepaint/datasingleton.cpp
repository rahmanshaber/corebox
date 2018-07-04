/*
CoreBox is combination of some common desktop apps.

CoreBox is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#include "datasingleton.h"


DataSingleton* DataSingleton::m_pInstance = 0;

DataSingleton::DataSingleton()
{
    mPrimaryColor = Qt::black;
    mSecondaryColor = Qt::white;
    mPenSize = 1;
    mTextFont = QFont("Times", 12);
    mCurrentInstrument = NONE_INSTRUMENT;
    mPreviousInstrument = NONE_INSTRUMENT;
    mIsInitialized = false;
}

DataSingleton* DataSingleton::Instance()
{
    if(!m_pInstance)
        m_pInstance = new DataSingleton;

    return m_pInstance;
}

