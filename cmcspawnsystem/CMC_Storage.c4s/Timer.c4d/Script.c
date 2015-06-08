/*-- Zeitanzeige --*/

#strict 2

local iTimer;

public func Set(int iTime) { iTimer = iTime; }

func Initialize() {
  SetVisibility(VIS_All);
  SetPosition(50,200,this);
  Local(0)=0;
  Local(1)=0;
  iTimer=60*5;
  return 1;
}

protected func Tick()
{
  if(iTimer > 0) iTimer--;
  var color = RGB(255,255,255);
  if(iTimer < 60) color=RGB(255,50,50);
 
  CustomMessage(Format("@%.2d:%.2d",iTimer/60,iTimer%60),this,-1,0,3,color);
 
  if(iTimer == 0)
  {
    for(var i = 0; i < GetPlayerCount(); i++)
    {
      var clonk = CreateObject(PCMK,0,0,GetPlayerByIndex(i));
      MakeCrewMember(clonk,GetPlayerByIndex(i));
      SelectCrew(GetPlayerByIndex(i),clonk);
      GameCallEx("RelaunchPlayer",GetPlayerByIndex(i),clonk, 0, GetPlayerTeam(GetPlayerByIndex(i)));
    }
    RemoveAll(BUCR);
    Sound("Trumpet");
    RemoveObject();
  }
}
