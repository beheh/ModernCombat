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

func ControlDig(object pByObj) {
   if(iCooldown > 0) return(1); 
   iCooldown=30*35;

   byObj = pByObj;

   ScheduleCall(this(),"Shoot",70,15);
}

public func Shoot()
{
   var iX=Sin(GetR(pCannon),34);
   var iY=-Cos(GetR(pCannon),34)-3;
   
   Sound("Blast2");
   var pProjectile=CreateObject(_GSS,iX,iY,GetOwner(byObj));
   SetXDir( Sin(GetR(pCannon)+RandomX(-2,2),RandomX(12,17)),pProjectile,1);
   SetYDir(-Cos(GetR(pCannon)+RandomX(-2,2),RandomX(12,17)),pProjectile,1);
   ObjectSetAction(pCannon,"Backdraft");
   CreateParticle("LightFlash",iX,iY,0,0,500,RGBa(255,255,255,32));
   var iSmoke1=14;
   while(iSmoke1) {CreateParticle("Smoke",iX,iY,0,RandomY(0,-20),RandomX(50,100),RGB(96,96,96)); iSmoke1--;}
   MuzzleFlash(RandomX(30,75),this(),iX,iY,GetR(pCannon));
   iCooldown -= 70;
}
