--ʹ�ü��ܺ���

function s2311401_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1004532 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 10073)
           end
     elseif TypeID == 1006555 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 30096)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2311401, 1, "s2311401_Cast")

