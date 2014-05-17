/*-- Neues Script --*/

#strict 2

#appendto LGHC

local fLocked;

public func Lock() {return fLocked = true;}
public func UnLock() {return fLocked = false;}
public func IsLocked() {return fLocked;}

public func ChangeSizeXY()
{
	if(IsLocked())
		return;
	
	return _inherited(...);
}

public func ChangeColor()
{
	if(IsLocked())
		return;
	
	return _inherited(...);
}

public func Update()
{
	if(IsLocked())
		return Draw();
	
	return _inherited(...);
}

public func Draw() {
  var dir;
  if(!bDir) dir = 1;
  else      dir = 1-GetDir(GetActionTarget())*2;
  
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  if(IsLocked())
  	sizemod = 100;
  
  var fsin, fcos, xoff, yoff;
  fsin = Sin(iR, 1000);
  fcos = Cos(iR, 1000);
  xoff = 0;
  yoff = +64;
  
  var width = iSizeX*sizemod/100, height = iSizeY*sizemod/100;
  
  SetObjDrawTransform(
    +fcos*width/1000, +fsin*height/1000, (((1000-fcos)*xoff - fsin*yoff)*height)/1000+iOffX*1000*dir,
    -fsin*width/1000, +fcos*height/1000, (((1000-fcos)*yoff + fsin*xoff - 64000)*height)/1000+iOffY*1000
  );
}
