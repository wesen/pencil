/*

Pencil - Traditional Animation Software
logging.h - Copyright (C) 2013 Manuel Odendahl <wesen@ruinwesen.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef LOGGING_H
#define LOGGING_H

class QFile;

class Logging
{
public:
    static bool LoadLoggingConfig(QFile &file);
private:
    Logging() {}
};

#endif // LOGGING_H
