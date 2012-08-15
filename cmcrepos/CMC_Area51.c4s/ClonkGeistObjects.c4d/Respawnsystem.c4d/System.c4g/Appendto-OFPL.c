/*-- Neues Script --*/

#strict 2

#appendto OFPL

public func GetSpawnPoint(int &iX, int &iY, string &szFunction, int iPlr)
{
  if(!GetLength(spawnpoints))
  {
   iY = -30;
   return;
  }
	
	var iX, iY, szFunction;
	szFunction = GetBestSpawnpoint(spawnpoints, iPlr, iX, iY)[2];
	
  /*var rnd = Random(GetLength(spawnpoints));
  iX = spawnpoints[rnd][0];
  iY = spawnpoints[rnd][1];
  szFunction = spawnpoints[rnd][2];*/
}
