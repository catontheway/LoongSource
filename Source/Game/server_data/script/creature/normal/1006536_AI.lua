--���ﺰ���ű�



--������������
function c1006536_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 30072)
       end	

end

aux.RegisterCreatureEvent(1006536, 4, "c1006536_OnDie")

