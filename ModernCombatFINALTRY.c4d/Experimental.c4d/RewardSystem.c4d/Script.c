/*-- Belohnungssystem --*/

#strict 2

local aData;

public func IsChooseable()	{return true;}


/* Initalisierung */

protected func Initialize()
{
  aData = CreateArray(0);
}

protected func Activate(iByPlayer)
{
  var outstring = Format("$ActualPoints$|"),
      points = Format("$Points$");
      
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    var plr = GetPlayerByIndex(i);
    outstring = Format("%s%s: %d %s|",outstring,GetTaggedPlayerName(plr),
                       GetPlayerStats("Totalpoints",plr), points);
  }
  
  MessageWindow(outstring, iByPlayer);
  return(1);
}

protected func InitializePlayer(iPlr)
{
  aData[iPlr] = CreateArray();
  aData[iPlr][0] = GetTaggedPlayerName(iPlr);
}

/*--Stellen
0 - Spielername
1 - Battlepoints
2 - Teampoints
3 - Negativepoints*/

public func SetPlayerStats(string szType, int iPlr,int iChange)
{
  if(szType == "Battlepoints") aData[iPlr][1] += iChange;
  if(szType == "Teampoints") aData[iPlr][2] += iChange;
  if(szType == "Negativepoints") aData[iPlr][3] += iChange;
}

public func GetPlayerStats(string szType, int iPlr)
{
  if(szType == "Name") return aData[iPlr][0];
  if(szType == "Battlepoints") return aData[iPlr][1];
  if(szType == "Teampoints") return aData[iPlr][2];
  if(szType == "Negativepoints") return aData[iPlr][3];
  if(szType == "Totalpoints") return (aData[iPlr][1]+aData[iPlr][2]+aData[iPlr][3]);
}

public func GetPlayerAmount()
{
  return GetLength(aData);
}

global func DoEvaluateStats()
{
  //Das Regelobjekt
  var database = FindObject(AR_A);
  
  if(!database) return 0;

  //Für jeden Spieler
  for(var i=0; i < database->GetPlayerAmount(); i++)
  {
   AddEvaluationData(Format("$Complete$",
                            database->GetPlayerStats("Name", i),
                            database->GetPlayerStats("Killpoints", i),
                            database->GetPlayerStats("Teampoints", i),
                            database->GetPlayerStats("Negativepoints", i)),0);
   AddEvaluationData(
     Format("$CompletePoints$",
            database->GetPlayerStats("Name", i),
            database->GetPlayerStats("Totalpoints", i)),0);
  }
}
