--���ﺰ���ű�

-- �������ս������
function c1003108_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 37)
       end	
end

aux.RegisterCreatureEvent(1003108, 2, "c1003108_OnEnterCombat")

