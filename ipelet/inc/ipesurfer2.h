#pragma once

#include <ipelet.h>
#include <ipepath.h>
#include <ipepage.h>
using namespace ipe;

IPELET_DECLARE Ipelet *newIpelet();

class KGonIpelet : public Ipelet {
public:
	virtual int ipelibVersion() const { return IPELIB_VERSION; }
	virtual bool run(int, IpeletData *data, IpeletHelper *helper);
};

IPELET_DECLARE Ipelet *newIpelet() {
	return new KGonIpelet;
}

