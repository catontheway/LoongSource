--���ﺰ���ű�

-- �������ս������
function c1003125_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20)
       end	
end

aux.RegisterCreatureEvent(1003125, 2, "c1003125_OnEnterCombat")

