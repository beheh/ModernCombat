/*-- Dämonen! >:D --*/

#strict 2

global func DemonInvasion(int iPower)//Rettet eure wertlosen Seelen!
{
  var goal;
  while(goal = FindObject2(Find_Category(C4D_Goal)))
    RemoveObject(goal);

  for(var i; i < iPower; i++)
    PlaceAnimal(RandomIndex4K([WIPF,WIPF,WIPF,MONS,SNKE,SNKE,BIRD,BIRD,FMNS,CIVC,CIVC,PCMK,MDIC]))->Demonize();

  Message("<c 00ff00>!DÄMONEN-INVASION!</c>|Es gibt jetzt %d Dämonen!",0,ObjectCount2(Find_Func("IsDemon")));
  
  var j;
  var clnk;
  while(clnk = GetCursor(j++))
    LaunchEarthquake(GetX(clnk),GetY(clnk),70);
}

global func IsDemon(object pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  return GetEffect("*Demon*",pTarget);
}

global func FxIntDemonStart(object pTarget, int iEffectNumber, int iTemp)
{
  //pTarget->SetName("Dämon");
  pTarget->SetPhysical("BreatheWater",1);

  pTarget->DoEnergy(-(GetEnergy(pTarget)/2));//*hust* Energie abziehen.

  EffectVar(1,pTarget,iEffectNumber) = pTarget->GetOwner();
  pTarget->SetOwner(-1);

  SetClrModulation(RGB(255),pTarget);//Dämonen sind rot, ist ja klar!
  //EffectVar(2,pTarget,iEffectNumber) = mod;//Modulation
  
  if(GetOwner(pTarget) > -1)
    if(GetOCF(pTarget) & OCF_CrewMember)
      SetCrewEnabled(false,pTarget);//Keine Kontrolle mehr!
      
  AddEffect("PossessionSpell", pTarget, 1);
}

global func FxIntDemonTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var enemy = EffectVar (0,pTarget,iEffectNumber);
  if(!enemy)
  {
    for(var obj in FindObjects(Sort_Distance(),
                               Find_Distance(500),
                               Find_OCF(OCF_Alive),
                               Find_NoContainer(),
                               Find_Not(Find_Func("IsDemon")),
                               Find_Exclude(pTarget)))
    {
      enemy = obj;//Wir haben einen ... auf ihn!!
      break;
    }
  }
  
  if(enemy)
  {
    if((ObjectDistance(pTarget,enemy) >= 500) || Contained(enemy))
    {
      EffectVar(0,pTarget,iEffectNumber) = 0;
      return ;
    }
    else
      EffectVar(0,pTarget,iEffectNumber) = enemy;
    
    var level = Min(GetEnergy(pTarget),70);
    if(ObjectDistance(pTarget,enemy) <= level*2/3)
    {
      var dmg = level/3;
      CreateParticle("PSpark",0,0,0,0,Distance(GetObjWidth(pTarget),GetObjHeight(pTarget))*5,RGB(255-Random(100),0,0));
      
      for(var obj in FindObjects(pTarget->Find_Distance(level),
                                 Find_Exclude(pTarget),
                                 Find_NoContainer(),
                                 Find_OCF(OCF_Alive),
                                 Find_Not(Find_Func("IsDemon")),
                                 Find_Category(C4D_Object|C4D_Living|C4D_Vehicle)))
      {
        var angle = Angle(GetX(pTarget),GetY(pTarget),GetX(obj),GetY(obj));
        var xdir = +Sin(angle,10);
        var ydir = -Cos(angle,10);
        CreateParticle("FSpark",0,0,xdir,ydir,dmg*5,RGB(255-Random(100),0,0));
        if(!Random(4))
        {
          Fling(obj,xdir/3,ydir/3);
          Fling(pTarget,-xdir/3,-ydir/3);
        }
        pTarget->DoDmg(dmg - (ObjectDistance(pTarget,obj)*dmg/level),DMG_Fire,obj,0,0,GetID(pTarget));
      }
    }
    else
      SetCommand(pTarget,"MoveTo",enemy);
  }
}

global func FxIntDemonStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(iReason >= 3)
  {
    var killer = GetKiller(pTarget);
    if(killer > -1)
      DoWealth(killer,GetValue(pTarget,0,0,killer)/6);
    //CreateObject(ROCK,0,0,GetOwner(pTarget))->Explode(Min(GetEnergy(pTarget),70));//Nicht übertreiben.
    SetClrModulation(RGB(255),pTarget);
  }
  else
  {
    //Reset
    RemoveEffect("PossessionSpell",pTarget);
    
    pTarget->SetPhysical("BreatheWater",0);
    
    pTarget->SetOwner(EffectVar(1,pTarget,iEffectNumber));
    
    SetCommand(pTarget,"No!=");
    
    SetClrModulation (0,pTarget);

    if(GetOwner(pTarget) > -1)
      if(GetOCF(pTarget) & OCF_CrewMember)
        SetCrewEnabled(true,pTarget);
  }
}

global func Demonize(pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  return AddEffect("IntDemon",pTarget,20,20,pTarget); 
}

global func UnDemonize(pTarget)
{
  if(!pTarget) pTarget = this;
  if(!pTarget) return false;
  
  return RemoveEffect("IntDemon",pTarget); 
}


/* temporäre Besitznahme-Überladung */
protected func FxPossessionSpellStart(pTarget, iEffectNumber, fTemp, pCaster){}
protected func FxPossessionSpellTimer(pTarget, iEffectNumber, iEffectTime){}
protected func FxPossessionSpellStop(pTarget, iEffectNumber, iReason, fTemp){}


//by Henry Kielmann aka [na:skar]
