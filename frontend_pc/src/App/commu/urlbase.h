#ifndef URLBASE_H
#define URLBASE_H

#endif // URLBASE_H

/* The file gives the definition of a base urls used in communication
 * Remeber to update this file once server address updates
 */

#include <QHash>

static QHash<QString, QString> urlbase{
    {"auth", "/v1/auth"},
    {"key2", "value2"}
};
