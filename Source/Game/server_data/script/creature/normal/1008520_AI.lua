--���ﺰ���ű�

-- �������ս������
function c1008520_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40072)
       end	
end

aux.RegisterCreatureEvent(1008520, 2, "c1008520_OnEnterCombat")

