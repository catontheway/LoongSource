--���ﺰ���ű�

-- �������ս������
function c1006301_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 30032)
       end	
end

aux.RegisterCreatureEvent(1006301, 2, "c1006301_OnEnterCombat")

