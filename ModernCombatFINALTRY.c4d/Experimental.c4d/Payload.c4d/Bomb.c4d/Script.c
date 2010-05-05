/*-- Bombe --*/

#strict 2

local iTeam, iDir, iTimer, iSpeed, iCheckpoint, aCheckpoints, iEnd, iCount;

/* Initialisierung */

public func Initialize() {
  iTeam = 0;
  iTimer = 0;
  iDir = DIR_Right;
  iSpeed = 3;
  iCheckpoint = -1;
  iEnd = 0;
  SetOwner(NO_OWNER);
}

/* Steuerung */

protected func ContactLeft() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
  if(!Stuck()) SetYDir(-2);
}

protected func ContactRight() {
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
  if(!Stuck()) SetYDir(-2);
}

/* Teams */

public func SetTeam(int iTeamNew) {
  iTeam = iTeamNew;
  SetOwner(NO_OWNER);
  SetColorDw(GetTeamColor(iTeam));
  return true;
}

public func GetTeam() {
  return iTeam;
}

public func SetCheckpoint(int iCheckpointNew) {
  iCheckpoint = iCheckpointNew;
  return true;
}

public func GetCheckpoint() {
  return iCheckpoint;
}

public func SetCheckpoints(array aCheckpointsNew) {
  aCheckpoints = aCheckpointsNew;
  return true;
}

public func SetEnd(iEndNew) {
  iEnd = iEndNew;
  return true;
}

public func GetNextCheckpoint() {
  return aCheckpoints[iCheckpoint+1];
}

public func GetLastCheckpoint() {
  return aCheckpoints[iCheckpoint];
}

/* Check */

public func Timer() {
  //Erst mal anhalten und saubermachen
  SetXDir(0);
  Message("", this);
  //Und schauen ob wir Team haben
  if(!iTeam) return;
  //Schon fertig?
  if(aCheckpoints[iCheckpoint+1] != 0 && GetX()*(iDir*2-1) >= aCheckpoints[iCheckpoint+1]*(iDir*2-1)) {
    iCheckpoint++;
    if(iCheckpoint > 0) {
      EventInfo4K(0, Format("Die <c %x>Bombe</c> hat einen Checkpoint erreicht", GetTeamColor(iTeam)), PLBM);
      GameCallEx("AddPayloadTime",3,30);
    }
  }
  if(GetX()*(iDir*2-1) >= iEnd*(iDir*2-1)) {
    EventInfo4K(0, Format("Die <c %x>Bombe</c> hat das Ziel erreicht", GetTeamColor(iTeam)), PLBM);
    Explode(50);
    return;
  }
  //Heilen
  if(!(iCount % 30)) Heal(); 
  iCount++;
  //Sonst weitermachen
  var clonks = FindObjects(Find_Distance(50),Find_OCF(OCF_Alive),Find_NoContainer(),Find_Func("IsClonk"));
  var iFriends = 0;
  var iEnemies = 0;
  for(clonk in clonks) {
    if(!PathFree(GetX(), GetY(), GetX(clonk), GetY(clonk))) continue;
    //Freund? Gut!
    if(GetPlayerTeam(GetOwner(clonk)) == iTeam) {
      iFriends++;
      continue;
    }
    //Nicht so gut...
    else {
      iEnemies++;
      break;
    }
  }
  if(iEnemies == 0 && iFriends > 0) {
    Message("@<c %x>%dx</c>", this, GetTeamColor(iTeam), iFriends);
    iFriends = Min(iFriends, 3);
    SetXDir(iFriends*iSpeed*(iDir*2-1));
    iTimer = 0;
  }
  else if(GetX()*(iDir*2-1) > aCheckpoints[iCheckpoint]*(iDir*2-1)) {
    if(iFriends == 0) {
      iTimer++;
		  if(iTimer > 38*5 && 15-iTimer/38 >= 0) {
		    Message("<c %x>%d</c>", this, GetTeamColor(iTeam), 15-iTimer/38);
		  }
		  if(iTimer > 38*15) {
		    SetXDir(-(iSpeed/2)*(iDir*2-1));
		  }
    }
    else {
      Message("<c %x>!</c>", this, GetTeamColor(iTeam), 15-iTimer/38);
    }
  } 
}

public func Heal() {
  var heal = 8; //Merke: 8-2 -> 1 Patient
  var Patients = [];
  for(var patient in FindObjects(Find_OCF(OCF_Alive),			//Patient am Leben?
                                 Find_Distance(50),			//In Reichweite?
                                 Find_NoContainer()))	//Verbündet?
  if(patient->~IsClonk())						//Patient ein Clonk?
  {
   if(GetEnergy(patient) < GetPhysical("Energy",0, patient)/1000)
   {
    if(patient == Contained()) continue;
    if(GetPlayerTeam(GetOwner(patient)) != iTeam) continue;
    //Bewirkt, dass bei mehr Patienten weniger gut geheilt wird
    heal = Max(heal-2, 2);
    Patients[GetLength(Patients)] = patient;
   }
  }

  for(var clonk in Patients)
  {
   if(!clonk)
    continue;
   else
   {
    DoEnergy(heal, clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+5*5,RGB(210,20,20),clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+10*5,RGB(210,20,120),clonk);
    clonk->CreateParticle("ShockWave",0,0,0,0,5*GetObjHeight(clonk)+15*5,RGB(210,20,220),clonk);
    Sound("FAPK_Healing*.ogg");
   }
  }
}
