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

#ifndef DATASINGLETON_H
#define DATASINGLETON_H

#include "easypaintenums.h"

#include <QColor>
#include <QSize>
#include <QString>
#include <QMap>
#include <QKeySequence>
#include <QFont>
#include <QSettings>


class DataSingleton
{
public:
    static DataSingleton* Instance();

    inline QColor getPrimaryColor() { return mPrimaryColor; }
    inline void setPrimaryColor(const QColor &color) { mPrimaryColor = color; }
    inline QColor getSecondaryColor() { return mSecondaryColor; }
    inline void setSecondaryColor(const QColor &color) { mSecondaryColor = color; }
    inline int getPenSize() { return mPenSize; }
    inline void setPenSize(const int &size) { mPenSize = size; }
    inline InstrumentsEnum getInstrument() { return mCurrentInstrument; }
    inline void setInstrument(const InstrumentsEnum &instrument) { mCurrentInstrument = instrument; mIsResetCurve = true; }
    inline InstrumentsEnum getPreviousInstrument() { return mPreviousInstrument; }
    inline void setPreviousInstrument(const InstrumentsEnum &instrument) { mPreviousInstrument = instrument; }
    inline bool getIsInitialized() { return mIsInitialized; }
    inline void setIsInitialized(const bool &isInitialized = true) { mIsInitialized = isInitialized; }
    inline QString getLastFilePath() { return mLastFilePath; }
    inline void setLastFilePath(const QString &lastFilePath) { mLastFilePath = lastFilePath; }
    inline QSize getWindowSize() { return mWindowSize; }
    inline void setWindowSize(const QSize &winSize) { mWindowSize = winSize; }
    inline QFont getTextFont() { return mTextFont; }
    inline void setTextFont(const QFont textFont) { mTextFont = textFont; }
    inline QMap<QString, QKeySequence> getFileShortcuts() { return mFileShortcuts; }
    inline QKeySequence getFileShortcutByKey(const QString &key) { return mFileShortcuts[key]; }
    inline void setFileShortcutByKey(const QString &key, const QKeySequence &value) { mFileShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getEditShortcuts() { return mEditShortcuts; }
    inline QKeySequence getEditShortcutByKey(const QString &key) { return mEditShortcuts[key]; }
    inline void setEditShortcutByKey(const QString &key, const QKeySequence &value) { mEditShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getInstrumentsShortcuts() { return mInstrumentsShortcuts; }
    inline QKeySequence getInstrumentShortcutByKey(const QString &key) { return mInstrumentsShortcuts[key]; }
    inline void setInstrumentShortcutByKey(const QString &key, const QKeySequence &value) { mInstrumentsShortcuts[key] = value; }
    inline QMap<QString, QKeySequence> getToolsShortcuts() { return mToolsShortcuts; }
    inline QKeySequence getToolShortcutByKey(const QString &key) { return mToolsShortcuts[key]; }
    inline void setToolShortcutByKey(const QString &key, const QKeySequence &value) { mToolsShortcuts[key] = value; }

    //Needs for correct work of Bezier curve instrument
    inline void setResetCurve(bool b) { mIsResetCurve = b; }
    inline bool isResetCurve() { return mIsResetCurve; }


private:
    DataSingleton();
    DataSingleton(DataSingleton const&){}

    static DataSingleton* m_pInstance;
    QColor mPrimaryColor,
           mSecondaryColor;
    int mPenSize;
    InstrumentsEnum mCurrentInstrument, mPreviousInstrument;
    QSize mBaseSize, mWindowSize;
    bool mIsInitialized;
    bool mIsResetCurve; /**< Needs to correct work of Bezier curve instrument */

    QString mLastFilePath; /* last opened file */
    QFont mTextFont;
    QMap<QString, QKeySequence> mFileShortcuts, mEditShortcuts, mInstrumentsShortcuts, mToolsShortcuts;

};

#endif // DATASINGLETON_H
