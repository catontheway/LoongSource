--ʹ�ü��ܺ���

function s2301801_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1003123 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 83)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2301801, 1, "s2301801_Cast")
