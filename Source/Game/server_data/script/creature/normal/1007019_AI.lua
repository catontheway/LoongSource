--���ﺰ���ű�

-- �������ս������
function c1007019_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 40088)
       end	
end

aux.RegisterCreatureEvent(1007019, 2, "c1007019_OnEnterCombat")

