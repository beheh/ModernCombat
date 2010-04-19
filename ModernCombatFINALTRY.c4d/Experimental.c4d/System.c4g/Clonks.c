/*-- Clonk --*/

#strict 2
/*
#appendto CLNK


local assistkiller;

protected func Initialize()
{
  assistkiller = [];
  for(var i=0; i < GetPlayerCount(); i++)
    assistkiller[i*2] = -1;
  _inherited();
}

//Verdammt ich hab vergessen wie das Zeug funktioniert O_o (...ah, jetzt aber langsam)
public func OnHit(int iChange, int iType, object pFrom)
{
  var iByPlayer = GetController(pFrom);
  for(var i=0; i < GetLength(assistkiller)/2; i++)
  {
    if(assistkiller[i*2] == iByPlayer)
      return assistkiller[i*2+1] += iChange;
    if(assistkiller[i*2] == -1)
    {
      assistkiller[i*2] = iByPlayer;
      assistkiller[i*2+1] = iChange;
      return 1;
    }
  }
  _inherited(...);
}

protected func Death()
{
  var Database = FindObject(AR_A);
  var killer = GetKiller();
  
  
  if(killer < 0 || killer == GetOwner())
  {
    Database -> SetPlayerStats("Negativepoints", GetOwner(), SuicidePoints());
    AddEffect("PointMessage",this,130,2,this,0,Format("{{%i}}->{{%i}} <c ff0000>%d</c>", GetID(), SKUL, SuicidePoints()));
  }
  
  //Ansonsten Killpunkte geben (und Todespunkte (und Assistkills))
  if( Hostile(killer,GetOwner()) )
  {
    Database -> SetPlayerStats("Battlepoints", killer, KillPoints());
    AddEffect("PointMessage",GetCursor(killer),130,2,GetCursor(killer),0,Format("{{%i}}->{{%i}} <c 00ff00>+%d</c>", GetID(), SKUL, KillPoints()));
    Database -> SetPlayerStats("Negativepoints", GetOwner(), DeathPoints());
    AddEffect("PointMessage",this,130,2,this,0,Format("{{%i}}->{{%i}} <c ff0000>%d</c>", GetID(), SKUL, DeathPoints()));
    
    //Dem mit dem meisten angerichteten Schaden neben dem Killer Assistpunkte geben
    var highest = CreateArray(2);
    for(var i = 0; i < GetLength(assistkiller)/2; i++)
      if(assistkiller[i*2+1] > highest[0])
      {
        highest[0] = assistkiller[i*2+1];
        highest[1] = assistkiller[i*2];
      }
    if(highest[1] != killer)
    {
      Database -> SetPlayerStats("Battlepoints", highest[1], AssistPoints());
      AddEffect("PointMessage",GetCursor(highest[1]),130,2,GetCursor(highest[1]),0,Format("{{%i}}->{{%i}} <c 00ff00>%d</c>", GetID(), SKUL, AssistPoints()));
    }  
  }
  
  //OMGOMG TEAMKILLERRRR!!! D:
  if( !Hostile(killer,GetOwner()) && killer != GetOwner() && !(killer < 0))
  {
    Database -> SetPlayerStats("Negativepoints", killer, TeamkillPoints());
    AddEffect("PointMessage",GetCursor(killer),130,2,GetCursor(killer),0,Format("{{%i}}->{{%i}} <c ff0000>%d</c>", GetID(), SKUL, TeamkillPoints()));
  }
  _inherited(...);
}

public func FxPointMessageStart(pTarget, iNo, iTemp, szString)
{
  Log("start");
  if(iTemp)
    return -1;
  Log("temp");
  //Vars
  EffectVar(0,pTarget,iNo) = szString; //Die Message
  EffectVar(1,pTarget,iNo) = GetX();   //Die alte X Position
  EffectVar(2,pTarget,iNo) = GetY();   //Die alte Y Position
  //Sound (hier einfügen) <----------------------------------------
}

public func FxPointMessageTimer(pTarget, iNo, iTime)
{
  CustomMessage(EffectVar(0,pTarget,iNo),
                pTarget,
                GetOwner(),
                EffectVar(1,pTarget,iNo)-GetX(),
                EffectVar(2,pTarget,iNo)-GetY()-iTime/5,
                RGBa(255,255,255,BoundBy(-300+iTime*5,0,255)),
                0,0,
                MSG_Multiple);
  if(-300+iTime*5 > 255)
    return -1;
}
                

//Falls Clonk angezündet...
/*
protected func Incineration(int iByPlayer)
{
  if(iByPlayer != GetOwner() && Hostile(iByPlayer,GetOwner()))
  {
    var Database = FindObject(AR_A);
    Database -> SetPlayerStats("Battlepoints", iByPlayer, BonusPoints("IncinerateClonk"));
    CreateObject(MH_A, 0, 0, iByPlayer)->Set(Format("{{%i}}->{{%i}} <c 00ff00>+%d</c>", GetID(), FLAM, BonusPoints("IncinerateClonk")),GetCursor(iByPlayer));
  }
  _inherited(...);
}
*/

