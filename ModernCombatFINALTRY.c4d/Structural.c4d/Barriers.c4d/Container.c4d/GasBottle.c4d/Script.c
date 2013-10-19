/*-- Gasflasche --*/

#strict 2
#include XBRL

local damaged;


/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(damaged || !this || GetDamage() < 40) return;
  InstaExplode(iPlr);
}

func InstaExplode(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Effekte
  Sound("BarrelImpact*.ogg");
  Sound("Fuse.wav");
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastParticles("MetalSplinter", 2+Random(3), 100, 0,0, 30,100,RGB(250,0,0));
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",4,30,0,0,100,300,RGBa(255,255,255,100),RGBa(255,255,255,130));
  if(GetEffectData(EFSM_ExplosionEffects) > 1) AddEffect("GSBL_Smoke",this,251,1,this);

  //Nach oben schleudern
  SetRDir(RandomX(-15,15));
  SetXDir(Sin(GetR(),45));
  SetYDir(-Cos(GetR(),45));
  
  //Sound
  Sound("MISL_Thrust.ogg",0,0,0,-1,1);

  //Explosion
  ScheduleCall(this, "BlowUp", 25,0,iPlr);
}

func BlowUp(int iPlr)
{
  //Splitter verschleudern
  for(var i = 5; i > 0; --i)
  {
    var angle = Interpolate4K(Random(360),Angle(GetXDir(),GetYDir()),0,40,BoundBy(Distance(GetXDir(),GetYDir()),0,40)) - 180;
    var ammo = CreateObject(SHRP,0,0,GetOwner());
    SetController(iPlr, ammo);
    ammo->Launch(angle,70+Random(30),100+Random(100),5,15,20);
  }

  //Verschwinden lassen
  FakeExplode(30, iPlr+1);
  RemoveObject();
}

/* Raucheffekt */

protected func FxGSBL_SmokeTimer(pTarget, iNo, iTime)
{
  if(!GBackLiquid())
  {
    var dist = Distance(0,0,GetXDir(),GetYDir());
    var maxx = +Sin(GetR(),dist/10);
    var maxy = -Cos(GetR(),dist/10);
    var ptrdist = 50;

    for(var i=0; i<dist; i+=ptrdist)
    {
      var x = -maxx*i/dist;
      var y = -maxy*i/dist;

      var rand = RandomX(-10,10);
      var xdir = +Sin(GetR()+rand,8);
      var ydir = -Cos(GetR()+rand,8);

      CreateParticle("Smoke3",x,y,xdir,ydir,RandomX(50,70),RGBa(255,255,255,100),0,0);
    }
  }
}

/* Aufschlag */ 

protected func Hit(int iPlr)
{
  Sound("BarrelImpact*.ogg");
  if(!damaged) return 1;
    BlowUp(iPlr);
  return 1;
}