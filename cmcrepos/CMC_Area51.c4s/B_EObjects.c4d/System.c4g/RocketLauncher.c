/*-- Raketenwerfer mit ungelenktem Modus --*/

#strict 2

#appendto RTLR

public func FMData1T2(int data)
{
  if(data == FT_Name)		return "$Unguided$";
  return FMData1(data);
}

public func Fire1T1()
{
  LaunchRocket(MISL,Contained()->~AimAngle(10),true);
}

public func Fire1T2()
{
  LaunchRocket(MISL,Contained()->~AimAngle(10));
}

public func LaunchRocket(id rid, int angle, bool fGuided)
{
  //Austritt bestimmen
  var user = GetUser();
  var x,y;
  user->WeaponEnd(x,y);

  //Rakete abfeuern
  var rocket = CreateObject(rid,x,y+10,GetController(user));
  rocket->Launch(angle, user, fGuided);
  Sound("RTLR_Launch*.ogg", 0, rocket);
  SetController(GetController(user), rocket);

  //Sicht auf Rakete
  if(fGuided) SetPlrView(GetController(user), rocket);
  pRocket = rocket;

  //Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  if(GetEffectData(EFSM_ExplosionEffects) > 1) Sparks(5,RGB(255,128),ax-x,ay-y);
  if(GetEffectData(EFSM_ExplosionEffects) > 0)
  {
    CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

    for(var i=0; i<20; ++i)
    {
      var rand = RandomX(-10,+10);
      CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(80,140),RGBa(220,200,180,0),0,0);

      var rand = RandomX(-10,+10);
      CreateParticle("BlastSpark1",ax+Sin(angle,rand),ay-Cos(angle,rand),
      		RandomX(0,2*xdir),RandomX(0,2*ydir),
      		RandomX(40,70),RGBa(220,200,180,0),0,0);
    }
  }
  Echo("RTLR_Echo.ogg");
}

private func Check()
{
  if(!pRocket || !Contained() || Contents(0, Contained()) != this || !Contained()->~IsClonk() || !pRocket->Guideable()) return;

  //Sicht auf existierende Rakete setzen
  if(Contained()->~IsAiming())
    if(pRocket)
      SetPlrView(GetOwner(Contained()), pRocket);
}
