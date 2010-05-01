/*-- Artillerie --*/

#strict

local pCannon;
local bRotate,bDirection;
local iCooldown;
local byObj;

func Initialize() {
  
  pCannon=CreateObject(_GES,0,32,-1);
  SetR(RandomX(-44,44),pCannon);
  return(1);
}

func Rotation() {

if(!bRotate) return(0);

if(GetR(pCannon)> 80) {bRotate=0; SetR(GetR(pCannon)-1,pCannon); Sound("CannonStop"); return(0);}
if(GetR(pCannon)<-80) {bRotate=0; SetR(GetR(pCannon)+1,pCannon); Sound("CannonStop"); return(0);}

if(bDirection==0) {Sound("CannonRotation"); SetR(GetR(pCannon)+1,pCannon);}
if(bDirection==1) {Sound("CannonRotation"); SetR(GetR(pCannon)-1,pCannon);}

}

func ControlRight() {
    bRotate=1;
    bDirection=0;
}

func ControlLeft() {
    bRotate=1;
    bDirection=1;
}

func ControlDown() {
    bRotate=0;
    Sound("CannonStop");
}

func ControlUp() {
    ControlDown();
}

func ControlDig(object pByObj)
{
  var iX = -AbsX(), iY = -AbsY(), iXDir = Sin(GetR(pCannon),14), iYDir = -Cos(GetR(pCannon),14);
  SimFlight(iX,iY,iXDir,iYDir,50,50,500);
  var target = CreateObject(ARCR,iX,iY,GetOwner(pByObj));
  SetVisibility(VIS_Owner,target);
  SetPlrView(GetOwner(pByObj),target);
  Sound("Info.ogg");
}

func ControlThrow(object pByObj) {
   if(iCooldown > 0) return(1); 
   iCooldown=30*35;

   byObj = pByObj;
   Sound("Info.ogg");

   //Partikeleffekt
   //Schedule("CreateParticle(\"PSpark\",5,-10,0,0,60,GetPlrColor(GetOwner(byObj)),this())",20,50);

   ScheduleCall(this(),"Shoot",70,15);
}

public func Shoot()
{
   var iX=Sin(GetR(pCannon),34);
   var iY=-Cos(GetR(pCannon),34)-3;
   
   Sound("Blast2");
   var pProjectile=CreateObject(_GSS,iX,iY,GetOwner(byObj));
   SetXDir( Sin(GetR(pCannon),RandomX(14,16)),pProjectile,1);
   SetYDir(-Cos(GetR(pCannon),RandomX(14,16)),pProjectile,1);
   ObjectSetAction(pCannon,"Backdraft");
   CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
   var iSmoke1=14;
   while(iSmoke1) {CreateParticle("Smoke",iX,iY,0,RandomX(0,-20),RandomX(50,100),RGB(96,96,96)); iSmoke1--;}
   MuzzleFlash(RandomX(30,75),this(),iX,iY,GetR(pCannon));
   iCooldown -= 70;
}
