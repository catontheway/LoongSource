--���ﺰ���ű�

-- �������ս������
function c1008322_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40062)
       end	
end

aux.RegisterCreatureEvent(1008322, 2, "c1008322_OnEnterCombat")

