--���ﺰ���ű�

-- �������ս������
function c1005306_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 20030)
       end	
end

aux.RegisterCreatureEvent(1005306, 2, "c1005306_OnEnterCombat")

