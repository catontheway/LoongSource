--���ﺰ���ű�

-- �������ս������
function x1005314_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID,20045)
       end	
end

aux.RegisterCreatureEvent(1005314, 2, "x1005314_OnEnterCombat")


--������������
function x1005314_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 20046)
       end	

end

aux.RegisterCreatureEvent(1005314, 4, "x1005314_OnDie")

