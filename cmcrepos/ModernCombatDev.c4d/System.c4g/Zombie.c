/* Die Zombies! >:D */

#strict 2

global func ZombieInvasion(int iPower)//X Zombies spawnen.
{
  for(var i; i < iPower; i++)
  {
    var zombie = PlaceAnimal(RandomIndex4K([CIVC,CIVC,CIVC,PCMK,MDIC]));
    zombie->SetColorDw(HSL(Random(256),255,127));
    zombie->Zombize();
  }
    
  Message("<c 00ff00>!ZOMBIE-INVASION!</c>|Es gibt jetzt %d Zombies!",0,ObjectCount2(Find_Func("IsZombie")));
}


global func Zombize(pTarget)//Ein Lebewesen zu einem Zombie machen.
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  if(!GetEffect("IntZombie",pTarget))
    return AddEffect("IntZombie",pTarget,20,35,pTarget); 
}

global func UnZombize(pTarget)//Einen Zombie zurückverwandeln.
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  return RemoveEffect("IntZombie",pTarget); 
}

global func IsZombie(object pTarget)//Ist das Objekt ein Zombie?
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  return GetEffect("*Zombie*",pTarget);
}

global func IsActiveZombie(object pTarget)//Ist das Objekt ein aktiver Zombie? (Schläft also nicht.)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  if(GetEffect("IntZombieStandBy",pTarget))
    return ;
  
  return GetEffect("Int*Zombie*",pTarget);
}



static const ZOMBIE_ViewRange = 300;//Soweit sehen Zombies.

/*
  Effektvariablen:
  0: aktueller Feind
  1: aktueller Corpse
  2: früherer Owner (Ist das wichtig?)
  3: Kampferfahrung
  4: Klasse (0 = Standardprozedur)
*/

global func FxIntZombieStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp) return ;
  
  pTarget->SetName("Zombie");//Sollte vllt. in ZombieInvasion() rein?

  EffectVar(2,pTarget,iEffectNumber) = pTarget->GetOwner();
  pTarget->SetOwner(NO_OWNER);//Enteignen.

  pTarget->SetPhysical("Walk",GetPhysical("Walk",0,0,GetID(pTarget))*(50+Random(40))/100,2);
  pTarget->SetPhysical("CanScale",1,2);
  pTarget->SetPhysical("CanHangle",1,2);
  pTarget->SetPhysical("CorrosionResist",1,2);

  SetClrModulation(RGBa(Random(100),Random(200)),pTarget);
  
  if(GetOwner(pTarget) > -1)
    if(GetOCF(pTarget) & OCF_CrewMember)
      SetCrewEnabled(false,pTarget);//Keine Kontrolle mehr!
      
  if(!GetEffect("PossessionSpell",pTarget))
    AddEffect("PossessionSpell", pTarget, 1);
  
  //EffectCall(pTarget,iEffectNumber,"Timer");//Erstes Update.
}

global func FxIntZombieTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!GetAlive(pTarget) || IsFakeDeath(pTarget)){Kill(pTarget); return -1;}//CMC-Support
  
  if(InLiquid(pTarget))
    pTarget->SetPhysical("BreatheWater",1,2);
  else
    pTarget->SetPhysical("BreatheWater",0,2);

  var enemy = EffectVar (0,pTarget,iEffectNumber);
  var corpse = EffectVar (1,pTarget,iEffectNumber);
  var class = EffectVar (4,pTarget,iEffectNumber);
  
  if(!enemy || !GetAlive(enemy))
  {
    for(var obj in FindObjects(Sort_Distance(),
                               Find_Distance(ZOMBIE_ViewRange),
                               Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember),
                               Find_NoContainer(),
                               Find_Not(Find_Func("IsZombie")),
                               Find_Exclude(pTarget)))
    {
      enemy = obj;//Wir haben einen ... auf ihn!!
      break;
    }
  }
  
  if(!corpse || GetAlive(corpse))
  {
    for(var obj in FindObjects(Sort_Distance(),
                               Find_Distance(ZOMBIE_ViewRange),
                               Find_OCF(OCF_Living|OCF_Prey),
                               Find_NoContainer(),
                               Find_Exclude(pTarget)))
    {
      if(GetAlive(obj))
        continue;
    
      corpse = obj;
      break;
    }
  }
  
  var corpse_dst;
  if(corpse)
    corpse_dst = ObjectDistance(pTarget,corpse);
    
  var enemy_dst;
  if(enemy)
    enemy_dst = ObjectDistance(pTarget,enemy);
  
  //Liegt da eine reanimierbare Leiche rum?
  if(corpse_dst < Max((enemy_dst+1)/2, corpse_dst))
  {
    if(!corpse || GetAlive(corpse))
      EffectVar (1,pTarget,iEffectNumber) = 0;
    else
    {
      EffectVar (1,pTarget,iEffectNumber) = corpse;
      
      if(corpse_dst <= Distance(GetObjWidth(pTarget),GetObjHeight(pTarget))/3*2)
      {
        if(!EffectVar(3,pTarget,iEffectNumber))
          EffectVar(3,pTarget,iEffectNumber) = 4;
        else
        {
          EffectVar(3,pTarget,iEffectNumber)--;
          if(EffectVar(3,pTarget,iEffectNumber) <= 0)
          {
            pTarget->SetAction("FlatUp");
            pTarget->SetPhase(6);

            SetAlive(true,corpse);
            DoEnergy(GetPhysical("Energy", 0, corpse)*50/100000, corpse);
            ObjectSetAction(corpse,"FlatUp",0,0,1);
            corpse->Sound("Breath");

            RemoveEffect("FadeOut",corpse);

            corpse->Zombize();//>:D

            EffectVar (1,pTarget,iEffectNumber) = 0;
            
            //EXP steigt.
            EffectVar(3,pTarget,iEffectNumber)++;
          }
        }
      }
      else
      {
        if(GetCommand(pTarget,1) != corpse)
          SetCommand(pTarget,"MoveTo",corpse,0,0,0,false);
      }
      
      return ;
    }
  }
  
  if(EffectVar(3,pTarget,iEffectNumber) == 4 && !class)
  {
    class = RandomIndex4K(["Berserker","Assassin"]);
    if(EffectCall(pTarget,iEffectNumber,Format("%sInit",class)));//Darf die Klasse aktiv sein? (Nebenbei wird auch ein Init gemacht... *cough*)
      EffectVar(4,pTarget,iEffectNumber) = class;
    EffectVar(3,pTarget,iEffectNumber)++;
  }
  
  //Gibts einen Feind? *sabber*
  if(enemy)
  {
    EffectVar (0,pTarget,iEffectNumber) = enemy;
  
    if((ObjectDistance(pTarget,enemy) >= ZOMBIE_ViewRange) || !GetAlive(enemy) || IsZombie(enemy) || Contained(enemy))
    {
      EffectVar (0,pTarget,iEffectNumber) = 0;
      return ;
    }
    
    if(class)
    {
      if(!EffectCall(pTarget,iEffectNumber,Format("%sAttack",class),enemy))
        if(GetCommand(pTarget,1) != enemy)
          SetCommand(pTarget,"MoveTo",enemy,0,0,0,true);
          
      return ;
    }
          
    if(enemy_dst <= Distance(GetObjWidth(pTarget),GetObjHeight(pTarget))/3*2)
    {
      pTarget->SetAction("Throw");
      Punch(enemy,5+Random(10));
      
      /*if(!Random(4))
      {
        var glob = CreateObject(GLOB,GetX(pTarget)-GetX(enemy),GetY(pTarget)-GetY(enemy),NO_OWNER);
        glob->Sound("Corrode");
        glob->HitLiving(enemy);
      }*/
      
      //EXP steigt.
      EffectVar(3,pTarget,iEffectNumber)++;
    }
    else
      if(GetCommand(pTarget,1) != enemy)
        SetCommand(pTarget,"MoveTo",enemy,0,0,0,true);
      
    return ;
  }
  
  //Es gibt nix zu tun?! Ô.o
  AddEffect("IntZombieStandBy",pTarget,20,35*5,pTarget);
}

global func FxIntZombieStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return ;
  
  EffectCall(pTarget,iEffectNumber,Format("%sEnd",EffectVar(4,pTarget,iEffectNumber)));

  var killer = GetKiller(pTarget);
  if(killer > -1)
    DoWealth(killer,GetValue (pTarget,0,0,killer)/10);//Hier kann toll die Schwierigkeit ansetzen.

  pTarget->SetName(GetName(0,GetID(pTarget)));
  
  RemoveEffect("PossessionSpell",pTarget);
  
  pTarget->SetPhysical("CorrosionResist",0,2);
  ResetPhysical(pTarget,"Breath"); 
  ResetPhysical(pTarget,"Walk"); 
  
  pTarget->SetOwner(EffectVar(2,pTarget,iEffectNumber));
  
  SetCommand(pTarget,"No!=");
  
  SetClrModulation (0,pTarget);

  if(GetOwner(pTarget) > -1)
    if(GetOCF(pTarget) & OCF_CrewMember)
      SetCrewEnabled(true,pTarget);
      
  pTarget->FadeOut();
}

//StandBy-Modus (spart zwar keinen Strom, aber Ressourcen)
global func FxIntZombieStandByStart(object pTarget, int iEffectNumber, int iTemp)
{
  ChangeEffect("IntZombie",pTarget,0,"IntZombie",0);//Timer ausschalten.
  SetCommand(pTarget,"No!=");
  SetAction("Dead");
}

global func FxIntZombieStandByTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  for(var obj in FindObjects(Sort_Distance(),
                             Find_Distance(ZOMBIE_ViewRange),
                             Find_OCF(OCF_Alive|OCF_Prey),Find_OCF(OCF_CrewMember),
                             Find_NoContainer(),
                             Find_Exclude(pTarget)))
  {
    if(GetEffect("*Zombie*",obj))
      continue;
      
    //*klingel* Aufstehen!
    ObjectSetAction(pTarget,"FlatUp",0,0,1);
    ChangeEffect("IntZombie",pTarget,0,"IntZombie",35);
    return -1;
  }
}

///Spezial-Klassen

//Berserker
global func FxIntZombieBerserkerInit(object pTarget, int iEffectNumber)
{
  Message("<c ff0000>B</c>",pTarget);
  Schedule("DoCon(1)",10,20,pTarget);//Werde groß und stark.

  pTarget->SetPhysical("Energy",GetPhysical("Energy",0,0,GetID(pTarget))*200/100,2);
  pTarget->DoEnergy(GetEnergy(pTarget)*2);
  
  pTarget->Sound("DeathGrowl");
  
  return 1;
}

global func FxIntZombieBerserkerAttack(object pTarget, int iEffectNumber, object enemy)
{
  if(ObjectDistance(pTarget,enemy) > Distance(GetObjWidth(pTarget),GetObjHeight(pTarget))/3*2)
    return ;

  pTarget->SetAction("Throw");
  Punch(enemy,30+Random(20));
  /*CreateObject(GLOB,GetX(pTarget)-GetX(enemy),GetY(pTarget)-GetY(enemy),NO_OWNER)->HitLiving(enemy);
  var glob = CreateObject(GLOB,GetX(pTarget)-GetX(enemy),GetY(pTarget)-GetY(enemy),NO_OWNER);
  glob->Sound("Corrode");
  glob->HitLiving(enemy);*/
  pTarget->Sound("Growl*");
  
  return 1;
}

global func FxIntZombieBerserkerEnd(object pTarget, int iEffectNumber)
{
  Schedule("DoCon(-1)",10,20,pTarget);
  ResetPhysical(pTarget,"Energy"); 
}

//Attentäter
global func FxIntZombieAssassinInit(object pTarget, int iEffectNumber)
{
  Message("<c ffff00>A</c>",pTarget);

  pTarget->SetPhysical("Walk",GetPhysical("Walk",0,0,GetID(pTarget))*150/100,2);//So ein Rusher!
  pTarget->SetPhysical("Energy",GetPhysical("Energy",0,0,GetID(pTarget))*50/100,2);
  pTarget->Sound("DeathGrowl");
  
  AddEffect("IntZAssassin",pTarget,10,20);
  
  return 1;
}

//Spzialtimer (Ist kein Callback vom Zombie!)
global func FxIntZAssassinTimer(object pTarget, int iEffectNumber)
{
  Message("*",pTarget);
  var w = GetObjWidth(pTarget);
  var h = GetObjHeight(pTarget);
  CreateParticle ("AlienPlasma",RandomX(-w/3,+w/3),RandomX(-h/3,+h/3),GetXDir(pTarget)/6,GetYDir(pTarget)/6,20+Random(80),RGBa(Random(50),100+Random(100),0,50+Random(100)),pTarget);
}

global func FxIntZombieAssassinAttack(object pTarget, int iEffectNumber, object enemy)
{
  if(ObjectDistance(pTarget,enemy) > Distance(GetObjWidth(pTarget),GetObjHeight(pTarget))*2)
    return ;

  /*for(var i = 7+Random(5), glob ; i ; i--)
  {
    glob = CreateObject(SLST, 0, 0, GetOwner());
    glob ->~ Launch(RandomX(-40,40), RandomX(-40,40), 6);
  }
  
  CastParticles("SlimeGrav", 10, 25, 0, 0, 20, 40, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));
  CastParticles("FrSprk", 30, 5, 0, 0, 70, 130, RGBa(0, 240, 0, 10), RGBa(20, 255, 20, 75));  
  Sound("SlimeHit");
  Sound("Poff");*/
  
  Kill(pTarget);
  
  pTarget->Explode(20);
  
  return 1;
}

global func FxIntZombieAssassinEnd(object pTarget, int iEffectNumber)
{
  ResetPhysical(pTarget,"Energy");
  ResetPhysical(pTarget,"Walk"); 
  RemoveEffect("IntZAssassin",pTarget);
}


/* temporäre Besitznahme-Überladung */
protected func FxPossessionSpellStart(pTarget, iEffectNumber, fTemp, pCaster){}
protected func FxPossessionSpellTimer(pTarget, iEffectNumber, iEffectTime){}
protected func FxPossessionSpellStop(pTarget, iEffectNumber, iReason, fTemp){}


//by Henry Kielmann aka [na:skar]
