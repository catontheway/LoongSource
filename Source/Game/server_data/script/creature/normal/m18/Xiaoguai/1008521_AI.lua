--���ﺰ���ű�

-- �������ս������
function c1008521_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40073)
       end	
end

aux.RegisterCreatureEvent(1008521, 2, "c1008521_OnEnterCombat")

