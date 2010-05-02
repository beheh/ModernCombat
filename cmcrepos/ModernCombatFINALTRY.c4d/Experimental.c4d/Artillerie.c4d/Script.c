/*-- Artillerie --*/

#strict

local pCannon;
local bRotate,bDirection;
local iCooldown;
local byObj;
local bAiming,byAimObj;

func Initialize() {
  
  pCannon=CreateObject(CNON,0,32,-1);
  iCooldown = 0;
  bAiming = false;
  SetR(RandomX(-44,44),pCannon);
  return(1);
}

func Rotation() {

  if(iCooldown <= 0)
    CreateParticle("PSpark",0,-4,0,0,40,RGB(0,255,0),this());
  else
  {
    iCooldown -= 3;
    CreateParticle("PSpark",0,-4,0,0,40,RGB(255,0,0),this());
  }

  if(!bRotate) return(0);

  if(GetR(pCannon)> 80) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return(0);}
  if(GetR(pCannon)<-80) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return(0);}

  if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
  if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}

  if(!bAiming) return(1);

  var iX = -AbsX()+Sin(GetR(pCannon),34), iY = -AbsY()-Cos(GetR(pCannon),34)-3, iXDir = Sin(GetR(pCannon),150), iYDir = -Cos(GetR(pCannon),150);
  if(!SimFlight(iX,iY,iXDir,iYDir,50,50,500,10))
    return(1);
  var target = CreateObject(ARCR,AbsX(iX),AbsY(iY),GetOwner(pByObj));
  SetVisibility(VIS_Owner,target);
  SetPlrView(GetOwner(pByObj),target);

}

func ControlRight(pByObj)
{
  bRotate=1;
  bDirection=0;
}

func ControlLeft(pByObj)
{
  bRotate=1;
  bDirection=1;
}

func ControlDown(pByObj)
{
  bRotate=0;
  Sound("CannonStop");
}

func ControlUp(pByObj)
{
  ControlDown();
}

func ControlDig(object pByObj)
{
  bAiming = !bAiming;
  byAimObj = pByObj;
  Sound("Info.ogg");
}

func Grabbed(pByObj, bGrab)
{
  Log("%v",bGrab);
  if(!bGrab)
    bAiming = false;
}

func ControlThrow(object pByObj)
{
  if(iCooldown > 0) return(Sound("Error",0,0,0,GetOwner(pByObj)+1)); 

  iCooldown=50*35;
  byObj = pByObj;
  Sound("Info.ogg");
  ScheduleCall(this(),"Shoot",70,10);
}

public func Shoot()
{
  var iX=Sin(GetR(pCannon),34);
  var iY=-Cos(GetR(pCannon),34)-3;
   
  Sound("Blast2");
  var pProjectile=CreateObject(_GSS,iX,iY,GetOwner(byObj));
  SetXDir( Sin(GetR(pCannon),RandomX(140,160)),pProjectile,10);
  SetYDir(-Cos(GetR(pCannon),RandomX(140,160)),pProjectile,10);
  ObjectSetAction(pCannon,"Backdraft");
  CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
  for(var i = 0; i < 14; i++)
  {
    CreateParticle("Smoke",iX,iY+RandomX(-20,20),0,0,RandomX(50,100),RGB(96,96,96));
    iSmoke1--;
  }
  MuzzleFlash(RandomX(30,75),this(),iX,iY,GetR(pCannon));
}
