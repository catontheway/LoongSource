--���ﺰ���ű�

-- �������ս������
function c1008111_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40056)
       end	
end

aux.RegisterCreatureEvent(1008111, 2, "c1008111_OnEnterCombat")

