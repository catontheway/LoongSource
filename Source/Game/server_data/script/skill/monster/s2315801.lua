--ʹ�ü��ܺ���

function s2315801_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1005433 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 20068)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2315801, 1, "s2315801_Cast")
