-- �������ս������
function c1008003_OnEnterCombat(MapID, InstanceID, CreatureID)
	local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
		cre.MonsterSay(MapID, InstanceID, CreatureID, 40023)
	end
end
aux.RegisterCreatureEvent(1008003, 2, "c1008003_OnEnterCombat")

--ɱ���ؾ����� 1008003 ���ʸ���Ӿbuff2016601

function c1008003_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.AddBuff(MapID, InstanceID, RoleID, 2016601, RoleID)
end
aux.RegisterCreatureEvent(1008003, 4, "c1008003_OnDie")



