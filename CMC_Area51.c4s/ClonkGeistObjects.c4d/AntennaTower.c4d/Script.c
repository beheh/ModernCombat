/*-- Sendemast --*/

#strict 2

local aRopeHolders;
local fDestroyed;

public func IsDestroyed()	{return fDestroyed;}


/* Initalisierung */

protected func Initialize()
{
  SetAction("Be");
  aRopeHolders = [];
}

/* Seilhalterungen */

public func AddRopeHolder(int iX1, int iY1, int iX2, int iY2, object pFakeRopeHolder1, object pFakeRopeHolder2)
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

  aRopeHolders[GetLength(aRopeHolders)] = [rh1, rh2, rope];

  return [rh1, rh2, rope];
}

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
  if(IsDestroyed() || change < 0)
    return false;

  if(WorkingRopesCount())
    DoDamage(-GetDamage());
  else if(GetDamage() > 200)
    Colapse();

  return true;
}

/* Zerstörung */

protected func Colapse()
{
  //Nur einmalig zerstörbar
  if(fDestroyed) return;
  fDestroyed = true;

  //Bodenerschütterung
  //ShakeObjects(GetX(), GetY(), 300);

  //Seile fallen ab und verschwinden
  for(var array in aRopeHolders)
  {
    if(!array)
      continue;

    if(array[0])
    {
      SetCategory(C4D_Object, array[0]);
      array[0]->FadeOut();
    }
    if(array[1])
    {
      SetCategory(C4D_Object, array[1]);
      array[1]->FadeOut();
    }
  }

  //Maststücke erstellen
  var part = CreateObject(ATRP, 0,-200, NO_OWNER);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, -2, 0);

  part = CreateObject(ATRP, 0,-50, NO_OWNER);
  part->SetPhase(1);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, 2, 0);

  part = CreateObject(ATRP, 0,100, NO_OWNER);
  part->SetPhase(2);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, -1, 0);

  part = CreateObject(ATRP, 0,250, NO_OWNER);
  part->SetPhase(3);
  part->SetRDir(RandomX(-5,5));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,-203, NO_OWNER);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,-22, NO_OWNER);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,27, NO_OWNER);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, 0,76, NO_OWNER);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  part = CreateObject(ATR2, -9,238, NO_OWNER);
  part->SetAction("Be2");
  part->SetR(180);
  part->SetRDir(RandomX(-4,4));
  part->Fling(part, 1, 0);

  //Aussehen anpassen
  SetPhase(1);

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