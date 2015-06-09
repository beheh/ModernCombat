/*-- Stalaktit --*/

#strict 2

local phase, y;
local pGrow;
local damaged;

public func IsBulletTarget()	{return !damaged;}


/* Initialisierung */

func Initialize()
{
  y = GetY()+38;

  //Aktion und Aussehen setzen
  SetAction("Hanging");
  SetPhase(phase=Random(4));
}

func Grow()
{
  SetObjDrawTransform(1000, 0, 0, 0, 0);
  pGrow=100;
}

func Check()
{
  var clnk;
  if(pGrow)
  {
    pGrow-=5;
    SetObjDrawTransform(1000, 0, 0, 0, 10*(100-pGrow), -pGrow*190);
    return 1;
  }
  if(GetAction()=="Falling") return Falling();

  y = GetY();
}

func Falling()
{
  var clnk;
  while(clnk=FindObject(0,-7,-19,14,50,OCF_CrewMember,0,0,0,clnk)) 
    DoEnergy(RandomX(-5,0),clnk);
}

/* Abbrechen */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  if(damaged)
    return;
  SetController(iPlr);

  if(!this)
    return;
  if(GetDamage() < 40) return;
  ScheduleCall(this, "BreakOff", 1,0,iPlr);
}

func BreakOff(int iPlr)
{
  if(damaged) return;
  damaged = true;

  //Komplettes Bruchstück erstellen
  CreateObject(GetID(), 0, 19)->Grow();

  SetAction("Falling");
  SetPhase(phase);
  RemoveVertex(0);
  RemoveVertex(0);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("ConcreteSplinter",10,100,0,0,20,100);
  CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,50),RGBa(255,255,255,130));
  Sound("RockBreak*");
  Sound("Blast2");
}

/* Aufprall */

func Hit()
{
  var obj;
  Sound("RockBreak*");
  Sound("Blast2",0,0,50);
  // links oben
  ObjectSetAction(obj=CreateObject(_STP,-4,-4,-1),"Exist");
  SetPhase(phase*2,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // rechts oben
  ObjectSetAction(obj=CreateObject(_STP,3,-4,-1),"Exist");
  SetPhase(phase*2+1,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(30)-15,-Random(35)+10,obj);
  // unten
  ObjectSetAction(obj=CreateObject(_STP,0,12,-1),"LameExist");
  SetPhase(phase,obj);
  SetRDir(Random(20)-10,obj);
  SetSpeed(Random(10)-15,-Random(35)+10,obj);
	
  RemoveObject();
}


/*

  
  if(!Random(2))
  for(clnk in FindObjects(Find_InRect(-25,0,50,100), Find_OCF(OCF_CrewMember, OCF_Alive)))
  { 
      CreateObject(GetID(), 0, 19)->Grow();
      Sound("Earthquake"); 
      SetAction("Falling");
      SetPhase(phase);
      RemoveVertex(0);
      RemoveVertex(0);
      return 1;
  }


*/