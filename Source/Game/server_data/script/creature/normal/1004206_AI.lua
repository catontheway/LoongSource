--���ﺰ���ű�

-- �������ս������
function c1004206_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10022)
       end	
end

aux.RegisterCreatureEvent(1004206, 2, "c1004206_OnEnterCombat")

