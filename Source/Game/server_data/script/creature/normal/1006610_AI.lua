--���ﺰ���ű�

-- �������ս������
function c1006610_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30123)
       end	
end

aux.RegisterCreatureEvent(1006610, 2, "c1006610_OnEnterCombat")
