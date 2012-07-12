/*-- Sendemast --*/

#strict 2

local aRopeHolders;
local fDestroyed;
local iLastDmgPlr;

public func IsDestroyed()	{return fDestroyed;}


/* Initalisierung */

protected func Initialize()
{
  SetAction("Stand");
  aRopeHolders = [];
  iLastDmgPlr = -1;
}

/* Seilhalterungen */

public func AddRopeHolder(int iX, int iY, int iPosition, int iRotation, int iGraphic)
{
	//Halterungen erstellen
  var rh1 = CreateObject(REHR, AbsX(iX), AbsY(iY), -1);
  var x, y;
  RopeHolderPosition(x, y, iPosition);
  x *= (AbsX(iX) >= 0) * 2 - 1;
  
  var rh2 = CreateObject(REHR, x, y, -1);
  
  //Befestigen
  rh1->SetAntenna(this);
  rh2->SetAntenna(this);
  var fakeropeholder = CreateObject(RTMP, AbsX(iX), AbsY(iY), -1);
  fakeropeholder->SetR(iRotation);
  fakeropeholder->SetGraphic(iGraphic);
  rh1->SetFakeRopeHolder(fakeropeholder);
  
  var rope = CreateObject(CK5P, 0, 0, -1);
  rope->ConnectObjects(rh1, rh2);
  rope->SetRopeLength(Distance(iX, iY, x, y));
	rope->SetRopeColor(RGB(100,100,100));

  aRopeHolders[GetLength(aRopeHolders)] = [rh1, rh2, rope];

  return [rh1, rh2, fakeropeholder, rope];
}

public func RopeHolderPosition(int &iX, int &iY, int iPosition)
{
	if(!iPosition)     { iX = 16; iY = -250; }
	if(iPosition == 1) { iX = 20; iY = 0; }
	if(iPosition == 2) { iX = 20; iY = 250; }
	if(iPosition == 3) { iX = 20; iY = 320; }

	return true;
}

/*public func AddRopeHolder(int iX1, int iY1, int iX2, int iY2, object pFakeRopeHolder1, object pFakeRopeHolder2)
{
  //Halterungen erstellen
  var rh1 = CreateObject(REHR, AbsX(iX1), AbsY(iY1), -1);
  var rh2 = CreateObject(REHR, AbsX(iX2), AbsY(iY2), -1);

  //Befestigen
  rh1->SetAntenna(this);
  rh2->SetAntenna(this);
  rh1->SetFakeRopeHolder(pFakeRopeHolder1);
  rh2->SetFakeRopeHolder(pFakeRopeHolder2);

  var rope = CreateObject(CK5P, 0, 0, -1);
  rope->ConnectObjects(rh1, rh2);
  rope->SetRopeLength(Distance(iX1, iY1, iX2, iY2));
	rope->SetRopeColor(RGB(100,100,100));

  aRopeHolders[GetLength(aRopeHolders)] = [rh1, rh2, rope];

  return [rh1, rh2, rope];
}*/

public func WorkingRopesCount()
{
  var cnt;
  for(var array in aRopeHolders)
  {
    if(!array)
      continue;

    if(!array[3])
      cnt++;
  }

  return cnt;
}

public func RopeHolderDestroyed(object pRopeHolder)
{
  for(var i = 0; i < GetLength(aRopeHolders); i++)
  {
    if(!aRopeHolders[i])
      continue;

    if(GetIndexOf(pRopeHolder, aRopeHolders[i]) > -1)
      aRopeHolders[i][3] = true;
  }
}

/* Schaden */

public func Damage(int change)
{
  //Kein Schaden wenn bereits zerstört
  if(IsDestroyed() || change < 0)
    return false;
  //Bei Seilhalterungen Schaden nullifizieren
  if(WorkingRopesCount())
    DoDamage(-GetDamage());
  //Ansonsten bei 200 Schaden zusammenfallen
  else if(GetDamage() > 200)
    CollapsePrepare();

  return true;
}

public func OnHit(int damage, int type, object pFrom)
{
  iLastDmgPlr = GetController(pFrom);
  return true;
}

/* Zerstörung */

protected func CollapsePrepare()
{
  //Nur einmalig zerstörbar
  if(fDestroyed) return;
  fDestroyed = true;

  //Aussehen anpassen
  SetPhase(1);

  //Vorhandene Seile fallen ab und verschwinden
  for(var array in aRopeHolders)
  {
    if(!array)
      continue;

    if(array[0])
    {
      SetCategory(C4D_Object, array[0]);
      array[0]->FadeOut();
      array[0]->SetOwner(iLastDmgPlr);
      array[0]->Sound("RopeBreakOff*.ogg");
    }
    if(array[1])
    {
      SetCategory(C4D_Object, array[1]);
      array[1]->FadeOut();
      array[1]->SetOwner(iLastDmgPlr);
      array[1]->Sound("RopeBreakOff*.ogg");
    }
  }

  //Zusammensturz planen
  ScheduleCall(0, "Collapse", 100);

  //Effekte
  CreateParticle("Smoke2",0,-200,-10,0,5*50);
  CreateParticle("Smoke2",0,-200,10,0,5*50);

  CreateParticle("Smoke2",0,-50,-10,0,5*50);
  CreateParticle("Smoke2",0,-50,10,0,5*50);

  CreateParticle("Smoke2",0,100,-10,0,5*50);
  CreateParticle("Smoke2",0,100,10,0,5*50);

  CreateParticle("Smoke2",0,250,-10,0,5*50);
  CreateParticle("Smoke2",0,250,10,0,5*50);
  
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, -200, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, -50, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, 100, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, 250, 20, 50);

  //Sound
  Sound("TowerBreaking.ogg");
}

protected func Collapse()
{
  //Aussehen anpassen
  SetPhase(2);

  //Bodenerschütterung
  //ShakeObjects(GetX(), GetY(), 300);

  //Maststücke erstellen
  var part = CreateObject(ATRP, 0,-200, iLastDmgPlr);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, -2, 0);

  part = CreateObject(ATRP, 0,-50, iLastDmgPlr);
  part->SetPhase(1);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, 2, 0);

  part = CreateObject(ATRP, 0,100, iLastDmgPlr);
  part->SetPhase(2);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, -1, 0);

  part = CreateObject(ATRP, 0,250, iLastDmgPlr);
  part->SetPhase(3);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,-203, iLastDmgPlr);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,-22, iLastDmgPlr);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,27, iLastDmgPlr);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,76, iLastDmgPlr);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, -9,238, iLastDmgPlr);
  part->SetAction("Be2");
  part->SetR(180);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  //Effekte
  CreateParticle("Blast",0,-200,-10,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,-200,10,0,5*50,RGB(255,255,128));

  CreateParticle("Blast",0,-50,-10,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,-50,10,0,5*50,RGB(255,255,128));

  CreateParticle("Blast",0,100,-10,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,100,10,0,5*50,RGB(255,255,128));

  CreateParticle("Blast",0,250,-10,0,5*50,RGB(255,255,128));
  CreateParticle("Blast",0,250,10,0,5*50,RGB(255,255,128));
  
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, -200, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, -50, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, 100, 20, 50);
  CastParticles("ConcreteSplinter", RandomX(8,16), 80, 0, 250, 20, 50);

  //Sound
  Sound("StructuralDamage*.ogg");
  Sound("BigExplosion.ogg");
}
