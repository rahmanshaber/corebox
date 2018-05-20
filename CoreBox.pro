TEMPLATE = subdirs

SUBDIRS += app \
           library

app.depends = library
