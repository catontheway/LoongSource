--���ﺰ���ű�

-- �������ս������
function c1003041_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 10)
       end
end

aux.RegisterCreatureEvent(1003041, 2, "c1003041_OnEnterCombat")

