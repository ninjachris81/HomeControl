#ifndef SHAREDLIB_H
#define SHAREDLIB_H

class SharedLib
{

#define INIT_RESOURCES() Q_INIT_RESOURCE(sharedcomponents); Q_INIT_RESOURCE(sharedutils); Q_INIT_RESOURCE(sharedfonts); Q_INIT_RESOURCE(sharedimages);

public:
    SharedLib();
};

#endif // SHAREDLIB_H
