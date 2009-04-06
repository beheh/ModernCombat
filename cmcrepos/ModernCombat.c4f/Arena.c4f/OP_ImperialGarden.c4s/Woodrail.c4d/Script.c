/*-- Woodrail --*/

#strict
#include RAI1

/* Status */

public func Index(int index) {
	if(index == 0) return("End1");
	if(index == 1) return("Linkage1");
	if(index == 2) return("Linkage2");
	if(index == 3) return("Backing");
	if(index == 4) return("End2");
}

public func Left()		{ return(0); }
public func Right()	{ return(4); }
public func Linkage(i)	{ return(1+(!!i)); }
public func Backing()	{ return(3); }