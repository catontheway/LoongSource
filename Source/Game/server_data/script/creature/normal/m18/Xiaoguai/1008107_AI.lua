--���ﺰ���ű�

-- �������ս������
function c1008107_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40054)
       end	
end

aux.RegisterCreatureEvent(1008107, 2, "c1008107_OnEnterCombat")

--������������
function c1008107_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, TargetID, 40055)
       end	
end
aux.RegisterCreatureEvent(1008107, 4, "c1008107_OnDie")


