/*-- Sendemast --*/

#strict 2


/* Initalisierung */

protected func Initialize()
{
  SetAction("Be");
}

protected func Colapse()
{
  //Bodenerschütterung
  //ShakeObjects(GetX(), GetY(), 300);

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

  //Sound
  Sound("StructuralDamage*.ogg");
  Sound("BigExplosion.ogg");
  Sound("TowerDestruction.ogg");
}