--���ﺰ���ű�

-- �������ս������
function c1005311_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20038)
       end	
end

aux.RegisterCreatureEvent(1005311, 2, "c1005311_OnEnterCombat")


--������������
function c1005311_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 20039)
       end	

end

aux.RegisterCreatureEvent(1005311, 4, "c1005311_OnDie")

